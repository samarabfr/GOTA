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

	DOREPLIFETIME(ASettlement, BuildingSummary);
	DOREPLIFETIME(ASettlement, CurrentBuildingProject);
	DOREPLIFETIME(ASettlement, Resources);
	DOREPLIFETIME(ASettlement, Affiliation);
}

ASettlement::ASettlement()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ISM_ClaimWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Claim Walls");
	ISM_ClaimWalls->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISM_ClaimWalls->SetupAttachment(RootComponent);
	ISM_ClaimWallsRiver = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Claim Walls River");
	ISM_ClaimWallsRiver->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISM_ClaimWallsRiver->SetupAttachment(RootComponent);
	
	PopulationSummary = CreateDefaultSubobject<USettlementPopulation>(TEXT("Population"));
	BuildingSummary = CreateDefaultSubobject<UBuildingSummary>(TEXT("Production"));
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

void ASettlement::GenerateIncome(float DeltaSeconds)
{
	Resources.Food += DeltaSeconds * BuildingSummary->ProductionMap[EProductionType::Food];
	Resources.Wood += DeltaSeconds * BuildingSummary->ProductionMap[EProductionType::Wood];
	Resources.Stone += DeltaSeconds * BuildingSummary->ProductionMap[EProductionType::Stone];
}

void ASettlement::OnBuildingAdded(UBuilding* Building)
{
	PopulationSummary->RegisterPop(Building->Population);
	BuildingSummary->RegisterBuildingProduction(Building);
}

void ASettlement::OnBuildingRemoved(UBuilding* Building)
{
	PopulationSummary->UnregisterPop(Building->Population);
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
	// TODO: make functions of income calc
	// food
	float FoodIncome = BuildingSummary->ProductionMap[EProductionType::Food];
	ImportanceRatings.Food = SettlementBalance->FoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->FoodImportanceDescent * FoodIncome);
	// wood
	float WoodIncome = BuildingSummary->ProductionMap[EProductionType::Wood];
	ImportanceRatings.Wood = SettlementBalance->WoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->WoodImportanceDescent * WoodIncome);
	// stone
	float StoneIncome = BuildingSummary->ProductionMap[EProductionType::Stone];
	ImportanceRatings.Stone = SettlementBalance->StoneImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementBalance->StoneImportanceDescent * StoneIncome);
}

void ASettlement::FigureOutSendingArmy()
{
	if (PopulationSummary->GetSize() < SettlementBalance->MinimumPopulationToSpawnArmy) return;

	// roll if army should spawn
	if (CalculateArmySpawnChance() < FMath::RandRange(0, 99)) return;
	// TODO: was losschicken? Wo spawnen? army spawnen; pop raus und rein
	SpawnArmy();
}

float ASettlement::CalculateArmySpawnChance()
{
	// angry ratio
	const float AngryRatio = PopulationSummary->GetAngry() / PopulationSummary->GetSize();
	const float AngryRatioImpact = AngryRatio * SettlementBalance->AggressiveMoodMaximumImpact;
	// Pop over high
	float PopOverHigh = PopulationSummary->GetSize() - SettlementBalance->HighPopulationThreshold;
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
