// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "Algo/RandomShuffle.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttribute.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttributeLimited.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASettlement, PopulationSummary);
	DOREPLIFETIME(ASettlement, BuildingSummary);
	DOREPLIFETIME(ASettlement, CurrentBuildingProject);
	DOREPLIFETIME(ASettlement, Food);
	DOREPLIFETIME(ASettlement, Wood);
	DOREPLIFETIME(ASettlement, Stone);
	DOREPLIFETIME(ASettlement, Expansion);
	DOREPLIFETIME(ASettlement, PrimaryCulture);
	DOREPLIFETIME(ASettlement, Affiliation);
}

ASettlement::ASettlement()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ISM_ClaimWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Claim Walls");
	ISM_ClaimWalls->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISM_ClaimWalls->SetupAttachment(RootComponent);
	ISM_ClaimWallsRiver = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Claim Walls River");
	ISM_ClaimWallsRiver->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISM_ClaimWallsRiver->SetupAttachment(RootComponent);

	// Replication stuff
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Food"));
	Wood = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Wood"));
	Stone = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Stone"));
	PopulationSummary = CreateDefaultSubobject<UPopulationSummary>(TEXT("Population"));
	BuildingSummary = CreateDefaultSubobject<UBuildingSummary>(TEXT("Production"));
	Expansion = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Expansion"));
	CurrentBuildingProject = CreateDefaultSubobject<UBuildingProject>(TEXT("Current Building Project"));
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();

	// Get the GameState
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	TileMap = GameState->TileMap;
	GameState->LoadingManager->IncrementReplicationCount();

	if (HasAuthority())
	{
		ISM_ClaimWalls->SetStaticMesh(ClaimMesh);

		AddReplicatedSubObject(Food);
		AddReplicatedSubObject(Wood);
		AddReplicatedSubObject(Stone);
		AddReplicatedSubObject(PopulationSummary);
		AddReplicatedSubObject(BuildingSummary);
		AddReplicatedSubObject(Expansion);
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}

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

void ASettlement::CalculateTurn()
{
	GenerateBaseIncome();
	GenerateBuildingIncome();
	FigureOutBuilding();
	FigureOutSendingArmy();
}

void ASettlement::OnBuildingAdded(UBuilding* Building)
{
	PopulationSummary->RegisterPopulationContainer(Building->PopContainer);
	BuildingSummary->RegisterBuildingProduction(Building);
}

void ASettlement::OnBuildingRemoved(UBuilding* Building)
{
	PopulationSummary->UnregisterPopulationContainer(Building->PopContainer);
	BuildingSummary->UnregisterBuildingProduction(Building);
}

void ASettlement::LostClaim(ATile* Tile)
{
	if (!Tile) return;

	ClaimedTiles.Remove(Tile);
}

FPrimitiveInstanceId ASettlement::AddClaimMeshInstance(FTransform& Transform)
{
	return ISM_ClaimWalls->AddInstanceById(Transform);
}

void ASettlement::RemoveClaimMeshInstance(FPrimitiveInstanceId InstanceId)
{
	ISM_ClaimWalls->RemoveInstanceById(InstanceId);
}

void ASettlement::ClaimTile(ATile* Tile)
{
	if (!Tile) return;

	if (Tile->TryClaim(this))
	{
		ClaimedTiles.Add(Tile);
	}
}

bool ASettlement::ClaimRandomTile()
{
	// All Neighboring tiles that have 3 or more neighbors already claimed by this settlement
	TArray<ATile*> BorderingTilesHighPrio;
	// All other claimable neighbors
	TArray<ATile*> BorderingTilesLowPrio;
	// Fill Bordering Arrays
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		for (int32 NeighborIndex = 0; NeighborIndex < 6; ++NeighborIndex)
		{
			// ignore this tile if its null or not claimable
			if (!ClaimedTile->Neighbors[NeighborIndex] || !ClaimedTile->Neighbors[NeighborIndex]->IsClaimable())
				continue;
			// check how many of this neighbor neighbors are claimed by this settlement
			int32 NeighborClaimedNeighbors = 0;
			for (int32 i = 0; i < 6; ++i)
			{
				if (ClaimedTile->Neighbors[NeighborIndex]->Neighbors[i]
					&& ClaimedTile->Neighbors[NeighborIndex]->Neighbors[i]->GetClaimant() == this)
				{
					NeighborClaimedNeighbors++;
				}
			}
			if (NeighborClaimedNeighbors >= 3)
			{
				BorderingTilesHighPrio.Add(ClaimedTile->Neighbors[NeighborIndex]);
			}
			else
			{
				BorderingTilesLowPrio.Add(ClaimedTile->Neighbors[NeighborIndex]);
			}
		}
	}
	if (!BorderingTilesHighPrio.IsEmpty())
	{
		// we have at least one High Prio claimable neighbor lets go
		ClaimTile(BorderingTilesHighPrio[FMath::RandRange(0, BorderingTilesHighPrio.Num() - 1)]);
		return true;
	}
	if (!BorderingTilesLowPrio.IsEmpty())
	{
		// well we at least have a low prio tile to claim, good enough
		ClaimTile(BorderingTilesLowPrio[FMath::RandRange(0, BorderingTilesLowPrio.Num() - 1)]);
		return true;
	}
	return false;
}

void ASettlement::GenerateBuildingIncomeAlly()
{
	int32 _;
	// Trees
	Wood->Add(TileMap->TryReduceEcoValue(this, EEcoValue::Tree,
	                                     *BuildingSummary->ProductionMap.Find(EProductionType::Woodcutting),
	                                     SettlementBalance->NativeTreeThreshold,
	                                     SettlementBalance->NativeMaxRange), _);
	// Wildlife
	Food->Add(TileMap->TryReduceEcoValue(this, EEcoValue::Wildlife,
	                                     *BuildingSummary->ProductionMap.Find(EProductionType::Hunting),
	                                     SettlementBalance->NativeWildlifeThreshold,
	                                     SettlementBalance->NativeMaxRange), _);
	// Forage
	Food->Add(TileMap->TryReduceEcoValue(this, EEcoValue::Forage,
	                                     *BuildingSummary->ProductionMap.Find(EProductionType::Foraging),
	                                     SettlementBalance->NativeForageThreshold,
	                                     SettlementBalance->NativeMaxRange), _);
}

void ASettlement::GenerateBuildingIncomeEnemy()
{
	int32 _;
	// Trees
	Wood->Add(TileMap->TryReduceEcoValue(this, EEcoValue::Tree,
	                                     *BuildingSummary->ProductionMap.Find(EProductionType::Woodcutting),
	                                     0,
	                                     SettlementBalance->ColonistMaxRange), _);
	// Wildlife
	Food->Add(TileMap->TryReduceEcoValue(this, EEcoValue::Wildlife,
	                                     *BuildingSummary->ProductionMap.Find(EProductionType::Hunting),
	                                     0,
	                                     SettlementBalance->ColonistMaxRange), _);
	// Forage
	Food->Add(TileMap->TryReduceEcoValue(this, EEcoValue::Forage,
	                                     *BuildingSummary->ProductionMap.Find(EProductionType::Foraging),
	                                     0,
	                                     SettlementBalance->ColonistMaxRange), _);
}

void ASettlement::GenerateBaseIncome()
{
	// Reached Expansion threshhold, base income +1
	if (Expansion->GetCurrent() == Expansion->GetMaximum())
	{
		ClaimRandomTile();
		Expansion->SetCurrent(1);
		Expansion->SetMaximum(FMath::TruncToInt32(
			SettlementBalance->ClaimPrice.GetRichCurveConst()->Eval(ClaimedTiles.Num())));
	}
	else
	{
		int32 _;
		Expansion->Add(1, _);
	}
}

void ASettlement::GenerateBuildingIncome()
{
	int32 _;
	Expansion->Add(*BuildingSummary->ProductionMap.Find(EProductionType::Expansion), _);
	if (Affiliation == EAffiliation::Ally)
	{
		GenerateBuildingIncomeAlly();
	}
	else
	{
		GenerateBuildingIncomeEnemy();
	}
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
		                                 BuildingProject->CalculateProjectTime(),
		                                 BuildingProject->Tier));
	}
	// sort by highest score
	Scores.Sort([](const FBuildingProjectScore& A, const FBuildingProjectScore& B)
	{
		return A.Score > B.Score;
	});
	// Set from highest score
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
			BuildingProject->Init(this, PossibleBuilding, 1, ClaimedTile);
			BuildingProjectPool.Add(BuildingProject);
		}
		break;
	}
	// look for ugprades
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		if (!ClaimedTile->Building) continue;

		// tier 2 is next and enabled
		if (ClaimedTile->Building->Tier == 1 && ClaimedTile->Building->DataAsset->TierTwo.TierEnabled)
		{
			UBuildingProject* BuildingProject = NewObject<UBuildingProject>();
			BuildingProject->Init(this, ClaimedTile->Building->DataAsset, 2, ClaimedTile);
			BuildingProjectPool.Add(BuildingProject);
		}
		// tier 3 is next and enabled
		else if (ClaimedTile->Building->Tier == 2 && ClaimedTile->Building->DataAsset->TierThree.TierEnabled)
		{
			UBuildingProject* BuildingProject = NewObject<UBuildingProject>();
			BuildingProject->Init(this, ClaimedTile->Building->DataAsset, 3, ClaimedTile);
			BuildingProjectPool.Add(BuildingProject);
		}
	}
}

void ASettlement::CalculateImportances()
{
	// The less income, the more important
	// TODO: make functions of income calc
	// food
	float FoodIncome = BuildingSummary->ProductionMap[EProductionType::Hunting];
	FoodIncome += BuildingSummary->ProductionMap[EProductionType::Foraging]
		* SettlementBalance->ForagingFoodToWoodRatio;
	ImportanceRatings.Food = SettlementBalance->FoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->FoodImportanceDescent * FoodIncome);
	// wood
	float WoodIncome = BuildingSummary->ProductionMap[EProductionType::Woodcutting];
	WoodIncome += BuildingSummary->ProductionMap[EProductionType::Foraging]
		* (1 - SettlementBalance->ForagingFoodToWoodRatio);
	ImportanceRatings.Wood = SettlementBalance->WoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->WoodImportanceDescent * WoodIncome);
	// stone
	float StoneIncome = BuildingSummary->ProductionMap[EProductionType::Stonecutting];
	ImportanceRatings.Stone = SettlementBalance->StoneImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->StoneImportanceDescent * StoneIncome);
	// More aggressive => weapons more important
	float AngryRatio = 0;
	if (PopulationSummary->Population.Size > 0)
		AngryRatio = PopulationSummary->GetMood(EMood::Angry) / PopulationSummary->Population.Size;
	float WeaponsIncome = BuildingSummary->ProductionMap[EProductionType::Bowmaking] + BuildingSummary->ProductionMap[
		EProductionType::Musketmaking];
	ImportanceRatings.Weapons = SettlementBalance->WeaponsImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->WeaponsImportanceDescent * WeaponsIncome) * (1 + AngryRatio);
	float ShieldsIncome = BuildingSummary->ProductionMap[EProductionType::Shieldmaking];
	ImportanceRatings.Shields = SettlementBalance->ShieldsImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->ShieldsImportanceDescent * ShieldsIncome) * (1 + AngryRatio);
}

void ASettlement::FigureOutSendingArmy()
{
	if (PopulationSummary->Population.Size < SettlementBalance->MinimumPopulationToSpawnArmy) return;

	// roll if army should spawn
	if (CalculateArmySpawnChance() < FMath::RandRange(0, 99)) return;
	// TODO: was losschicken? Wo spawnen? army spawnen; pop raus und rein
	SpawnArmy();
}

float ASettlement::CalculateArmySpawnChance()
{
	// angry ratio
	const float AngryRatio = PopulationSummary->GetMood(EMood::Angry) / PopulationSummary->Population.Size;
	const float AngryRatioImpact = AngryRatio * SettlementBalance->AggressiveMoodMaximumImpact;
	// Pop over high
	float PopOverHigh = PopulationSummary->Population.Size - SettlementBalance->HighPopulationThreshold;
	PopOverHigh = FMath::Max(PopOverHigh, 0);
	const float PopOverHighImpact = PopOverHigh * SettlementBalance->HighPopulationImpact;
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
	AArmy* Army = Cast<AArmy>(GetWorld()->SpawnActor(ArmyClass));
	Army->RandomizePop();
	if (!Army) return false;
	Army->Init(Affiliation, SpawnLocation, 2);

	// reduce Pop in every building
	// TODO: evaluate how many pops to send
	// TODO: figure out which pops to send, remove them from the buildings and add them to the army
	for (ATile* Tile : ClaimedTiles)
	{
		if (!Tile || !Tile->Building) continue;
		Tile->Building->PopContainer->ChangeSize(-1);
	}

	return true;
}
