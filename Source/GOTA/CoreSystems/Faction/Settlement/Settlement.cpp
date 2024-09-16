// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "Algo/RandomShuffle.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Affiliation, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, BuildingSummary, Params);
	DOREPLIFETIME_WITH_PARAMS(ASettlement, CurrentBuildingProject, Params);
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Resources, Params);
}

ASettlement::ASettlement()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");

	PopulationSummary = CreateDefaultSubobject<USettlementPopulation>(TEXT("Population"));
	BuildingSummary = CreateDefaultSubobject<UBuildingSummary>(TEXT("Production"));
	CurrentBuildingProject = CreateDefaultSubobject<UBuildingProject>(TEXT("Current Building Project"));

	// Load Settlement Settings DataAsset
	ConstructorHelpers::FObjectFinder<USettlementSettings> DataAsset(
		TEXT("/Game/CoreSystems/Faction/DA_SettlementSettings"));
	SettlementSettings = DataAsset.Object;
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->LoadingManager->IncrementReplicationCount();

	if (HasAuthority())
	{
		AddReplicatedSubObject(BuildingSummary);
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}

void ASettlement::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GenerateIncome(DeltaSeconds);
	FigureOutBuilding();
	FigureOutSendingArmy();
}

void ASettlement::StartingSetup(ATile* SpawnTile)
{
	const FGameResources& StartingResources = Affiliation == EAffiliation::Enemy
		                                          ? SettlementSettings->C_StartingResources
		                                          : SettlementSettings->N_StartingResources;
	const TArray<UBuildingDataAsset*>& StartingBuildings = Affiliation == EAffiliation::Enemy
		                                                       ? SettlementSettings->C_StartingBuildings
		                                                       : SettlementSettings->N_StartingBuildings;
	GameplayTags = Affiliation == EAffiliation::Enemy
		               ? SettlementSettings->C_GameplayTags
		               : SettlementSettings->N_GameplayTags;
	Resources += StartingResources;
	SpawnTile->TryBuild(StartingBuildings[0], this);
	for (int32 i = 1; i < StartingBuildings.Num(); ++i)
	{
		if (BorderingUnclaimedTiles.Num() <= 0) break;
		BorderingUnclaimedTiles[FMath::RandRange(0, BorderingUnclaimedTiles.Num() - 1)]
			->TryBuild(StartingBuildings[i], this);
	}
}

void ASettlement::GenerateIncome(float DeltaSeconds)
{
	Resources.Food += DeltaSeconds * BuildingSummary->ProductionMap[EProductionType::Food];
	Resources.Wood += DeltaSeconds * BuildingSummary->ProductionMap[EProductionType::Wood];
	Resources.Stone += DeltaSeconds * BuildingSummary->ProductionMap[EProductionType::Stone];
}

// -------------------Claims-------------------------

void ASettlement::RefreshBorderingUnclaimedTiles()
{
	BorderingUnclaimedTiles.Empty();
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		for (ATile* Neighbor : ClaimedTile->Neighbors)
		{
			if (Neighbor && !Neighbor->GetClaimant() && !BorderingUnclaimedTiles.Contains(Neighbor))
			{
				BorderingUnclaimedTiles.Add(Neighbor);
			}
		}
	}
}

// -------------------Building-------------------------

void ASettlement::SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject)
{
	if (CurrentBuildingProject)
	{
		RemoveReplicatedSubObject(CurrentBuildingProject);
	}
	CurrentBuildingProject = NewCurrentBuildingProject;
	if (CurrentBuildingProject)
	{
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}

void ASettlement::OnBuildingAdded(UBuilding* Building, ATile* Tile)
{
	PopulationSummary->RegisterPop(Building->Population);
	BuildingSummary->RegisterBuildingProduction(Building);
	ClaimedTiles.Add(Tile);
	RefreshBorderingUnclaimedTiles();
}

void ASettlement::OnBuildingRemoved(UBuilding* Building, ATile* Tile)
{
	PopulationSummary->UnregisterPop(Building->Population);
	BuildingSummary->UnregisterBuildingProduction(Building);
	ClaimedTiles.Remove(Tile);
	RefreshBorderingUnclaimedTiles();
}

void ASettlement::FigureOutBuilding()
{
	if (CurrentBuildingProject && CurrentBuildingProject->IsPossible())
	{
		if (CurrentBuildingProject->CanAfford())
		{
			CurrentBuildingProject->TryBuilding();
			SetCurrentBuildingProject(nullptr);
			SelectNewBuildingProject();
		}
	}
	else
	{
		SetCurrentBuildingProject(nullptr);
		SelectNewBuildingProject();
	}
}

void ASettlement::SelectNewBuildingProject()
{
	CalculateImportances();
	FillBuildingPool();
	if (BuildingProjectPool.IsEmpty()) return;
	// calculate scores
	Scores.Empty();
	for (UBuildingProject* BuildingProject : BuildingProjectPool)
	{
		Scores.Add(FBuildingProjectScore(BuildingProject,
		                                 BuildingProject->CalculateScore(),
		                                 BuildingProject->Data,
		                                 BuildingProject->CalculateProjectTime()));
	}
	// sort by highest score
	Scores.Sort([](const FBuildingProjectScore& A, const FBuildingProjectScore& B)
	{
		return A.Score > B.Score;
	});
	// Set from the highest score
	UBuildingProject* Highest = nullptr;
	float LowestScore = -MAX_FLT;
	for (auto Score : Scores)
	{
		if (Score.Score > LowestScore)
		{
			LowestScore = Score.Score;
			Highest = Score.BuildingProject;
		}
	}
	SetCurrentBuildingProject(Highest);
}

void ASettlement::FillBuildingPool()
{
	BuildingProjectPool.Empty();
	// If there is a free building slot, add all possible Buildings to the pool
	Algo::RandomShuffle(ClaimedTiles);
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		if (ClaimedTile->Building) continue;
		for (UBuildingDataAsset* PossibleBuilding : PossibleBuildings)
		{
			UBuildingProject* BuildingProject = NewObject<UBuildingProject>();
			BuildingProject->Init(this, PossibleBuilding, ClaimedTile);
			BuildingProjectPool.Add(BuildingProject);
		}
		break;
	}
}

void ASettlement::CalculateImportances()
{
	// The less income, the more important
	// food
	float FoodIncome = BuildingSummary->ProductionMap[EProductionType::Food];
	ImportanceRatings.Food = SettlementSettings->FoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementSettings->FoodImportanceDescent * FoodIncome);
	// wood
	float WoodIncome = BuildingSummary->ProductionMap[EProductionType::Wood];
	ImportanceRatings.Wood = SettlementSettings->WoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementSettings->WoodImportanceDescent * WoodIncome);
	// stone
	float StoneIncome = BuildingSummary->ProductionMap[EProductionType::Stone];
	ImportanceRatings.Stone = SettlementSettings->StoneImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementSettings->StoneImportanceDescent * StoneIncome);
}

// -------------------Army??-------------------------

void ASettlement::FigureOutSendingArmy()
{
	if (PopulationSummary->GetSize() < SettlementSettings->MinimumPopulationToSpawnArmy) return;

	// roll if army should spawn
	if (CalculateArmySpawnChance() < FMath::RandRange(0, 99)) return;
	// TODO: was losschicken? Wo spawnen? army spawnen; pop raus und rein
	SpawnArmy();
}

float ASettlement::CalculateArmySpawnChance()
{
	// angry ratio
	const float AngryRatio = PopulationSummary->GetAngry() / PopulationSummary->GetSize();
	const float AngryRatioImpact = AngryRatio * SettlementSettings->AggressiveMoodMaximumImpact;
	// Pop over high
	float PopOverHigh = PopulationSummary->GetSize() - SettlementSettings->HighPopulationThreshold;
	PopOverHigh = FMath::Max(PopOverHigh, 0);
	const float PopOverHighImpact = PopOverHigh * SettlementSettings->HighPopulationImpact;
	// spawn chance
	return AngryRatioImpact + PopOverHighImpact;
}

bool ASettlement::SpawnArmy()
{
	// nowhere to spawn
	if (ClaimedTiles.IsEmpty()) return false;
	// first Tile that has no TileEntity with own affiliation
	ATile* SpawnLocation = nullptr;
	for (ATile* Tile : ClaimedTiles)
	{
		if (!Tile || Tile->GetEntity(Affiliation)) continue;
		SpawnLocation = Tile;
		break;
	}
	if (!SpawnLocation) return false;

	// spawn the army
	AArmy* Army;
	if (Affiliation == EAffiliation::Enemy)
	{
		Army = Cast<AArmy>(GetWorld()->SpawnActor(SettlementSettings->C_ArmyClass));
	}
	else
	{
		Army = Cast<AArmy>(GetWorld()->SpawnActor(SettlementSettings->N_ArmyClass));
	}
	if (!Army) return false;
	Army->Init(Affiliation, SpawnLocation, 2);

	// reduce Pop in every building
	// TODO: evaluate how many pops to send
	// TODO: figure out which pops to send, remove them from the buildings and add them to the army
	for (ATile* Tile : ClaimedTiles)
	{
		if (!Tile || !Tile->Building) continue;
		Tile->Building->Population->ChangeSize(-1);
	}

	return true;
}
