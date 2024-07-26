// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "HairStrandsInterface.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "GOTA/Faction/Building.h"
#include "Net/UnrealNetwork.h"

bool ATile::bFreezeGrowthChanges = false;

//Unreal Engine Mystery Code
void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATile, HexCoords);
	DOREPLIFETIME(ATile, Claimant);
	DOREPLIFETIME(ATile, Trees);
	DOREPLIFETIME(ATile, TreeGrowth);
	DOREPLIFETIME(ATile, TreeGrowthChange);
	DOREPLIFETIME(ATile, Forage);
	DOREPLIFETIME(ATile, ForageChange);
	DOREPLIFETIME(ATile, Wildlife);
	DOREPLIFETIME(ATile, WildlifeGrowth);
	DOREPLIFETIME(ATile, WildlifeGrowthChange);
	DOREPLIFETIME(ATile, Building);
	DOREPLIFETIME(ATile, Neighbors);
	DOREPLIFETIME(ATile, AlliedTileEntity);
	DOREPLIFETIME(ATile, EnemyTileEntity);
	DOREPLIFETIME(ATile, bIsRiver);
	DOREPLIFETIME(ATile, SpawnPointLayout);
}

// Constructor
ATile::ATile()
{
	// initialize neighbor array
	for (int i = 0; i < 6; ++i)
	{
		Neighbors.Add(nullptr);
	}

	// Replication stuff
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	Trees = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Trees"));
	TreeGrowth = CreateDefaultSubobject<UGOTAAttribute>(TEXT("TreeGrowth"));
	TreeGrowthChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("TreeGrowthChange"));
	Forage = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Forage"));
	ForageChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("ForageChange"));
	Wildlife = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Wildlife"));
	WildlifeGrowth = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WildlifeGrowth"));
	WildlifeGrowthChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WildlifeGrowthChange"));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	// Get the GameState
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();

	if (HasAuthority())
	{
		AddReplicatedSubObject(Trees);
		AddReplicatedSubObject(TreeGrowth);
		AddReplicatedSubObject(TreeGrowthChange);
		AddReplicatedSubObject(Forage);
		AddReplicatedSubObject(ForageChange);
		AddReplicatedSubObject(Wildlife);
		AddReplicatedSubObject(WildlifeGrowth);
		AddReplicatedSubObject(WildlifeGrowthChange);
	}
}

void ATile::Init()
{
	Trees->OnChanged.AddDynamic(this, &ATile::CalculateTreeGrowthChangeWithNeighbors);
	Trees->SetMaximum(BalanceData->MaxTrees);
	Trees->SetCurrent(BalanceData->StartingTrees);
	Forage->OnChanged.AddDynamic(this, &ATile::CalculateForageChangeWithNeighbors);
	Forage->SetMaximum(BalanceData->MaxForage);
	Forage->SetCurrent(BalanceData->StartingForage);
	Wildlife->OnChanged.AddDynamic(this, &ATile::CalculateWildlifeGrowthChangeWithNeighbors);
	Wildlife->SetMaximum(BalanceData->MaxWildlife);
	Wildlife->SetCurrent(BalanceData->StartingWildlife);
	RefreshTileLayout();
}

//====================================================================
//--------------------Claimant
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::CalculateTurn()
{
	int32 _;
	// apply TreeGrowthChange
	TreeGrowth->Add(TreeGrowthChange->Current, _);
	// grow trees
	if (TreeGrowth->Current > BalanceData->TreeGrowthThreshold)
	{
		const int32 TreeGrowCount = TreeGrowth->Current / BalanceData->TreeGrowthThreshold;
		Trees->Add(TreeGrowCount, _);
		TreeGrowth->Subtract(TreeGrowCount * BalanceData->TreeGrowthThreshold, _);
	}
	// apply ForageChange
	int32 ForageEffectiveChange = 0;
	Forage->Add(ForageChange->Current, ForageEffectiveChange);
	// wildlife starvation
	if (ForageEffectiveChange < 0)
	{
		Wildlife->Add(ForageEffectiveChange, _);
	}
	// apply WildlifeGrowthChange
	WildlifeGrowth->Add(WildlifeGrowthChange->Current, _);
	// grow Wildlife
	if (WildlifeGrowth->Current > BalanceData->WildlifeGrowthThreshold)
	{
		const int32 WildlifeGrowCount = WildlifeGrowth->Current / BalanceData->WildlifeGrowthThreshold;
		Wildlife->Add(WildlifeGrowCount, _);
		WildlifeGrowth->Subtract(WildlifeGrowCount * BalanceData->WildlifeGrowthThreshold, _);
	}
	if (!Building) return;
	// apply PopulationGrowthChange
	Building->Population->Growth += Building->Population->GrowthChange;
	// grow Wildlife
	if (Building->Population->Growth > Building->Population->GrowthThreshold)
	{
		const int32 PopulationGrowCount = Building->Population->Growth / Building->Population->GrowthThreshold;
		Building->Population += PopulationGrowCount;
		Building->Population->Growth -= PopulationGrowCount * Building->Population->GrowthThreshold;
	}
}

void ATile::CalculateTreeGrowthChange()
{
	TreeGrowthChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i])
		{
			TreeGrowthChange->Add(Neighbors[i]->Trees->Current, _);
		}
	}
	TreeGrowthChange->Add(Trees->Current, _);
}

void ATile::CalculateTreeGrowthChangeWithNeighbors(int32 Change)
{
	if (bFreezeGrowthChanges) return;

	CalculateTreeGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculateTreeGrowthChange();
	}
}

void ATile::CalculateForageChange()
{
	ForageChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i])
		{
			ForageChange->Add(Neighbors[i]->Trees->Current * BalanceData->ForagePerNeighboringTree, _);
			ForageChange->Add(Neighbors[i]->ForageChange->Current * BalanceData->ForagePerNeighboringForage, _);
		}
	}
	ForageChange->Add(Trees->Current * BalanceData->ForagePerTree, _);
	ForageChange->Add(ForageChange->Current * BalanceData->ForagePerForage, _);
	ForageChange->Subtract(Wildlife->Current, _);
}

void ATile::CalculateForageChangeWithNeighbors(int32 Change)
{
	if (bFreezeGrowthChanges) return;

	CalculateForageChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculateForageChange();
	}
}

void ATile::CalculateWildlifeGrowthChange()
{
	WildlifeGrowthChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i])
		{
			WildlifeGrowthChange->Add(Neighbors[i]->Wildlife->Current, _);
		}
	}
	WildlifeGrowthChange->Add(Wildlife->Current, _);
}

void ATile::CalculateWildlifeGrowthChangeWithNeighbors(int32 Change)
{
	if (bFreezeGrowthChanges) return;

	CalculateWildlifeGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculateWildlifeGrowthChange();
	}
}

void ATile::CalculatePopulationGrowthChange()
{
	if (!Building) return;

	Building->Population->Growth = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i] && Neighbors[i]->Building)
		{
			Building->Population->Growth += Neighbors[i]->Building->Population->Current;
		}
	}
	Building->Population->Growth += Building->Population->Current;
}

void ATile::CalculatePopulationGrowthChangeWithNeighbors()
{
	if (bFreezeGrowthChanges) return;

	CalculatePopulationGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculatePopulationGrowthChange();
	}
}

bool ATile::IsWalkable(EAffiliation Affiliation) const
{
	if (Affiliation == EAffiliation::Ally)
	{
		return !EnemyTileEntity;
	}
	if (Affiliation == EAffiliation::Enemy)
	{
		return !AlliedTileEntity;
	}
	return false;
}

bool ATile::IsClaimable() const
{
	return !Claimant;
}

void ATile::AddBuildingToReplication()
{
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->Population);
	AddReplicatedSubObject(Building->Production);

	// TODO: Not Here
	Building->Population->OnChanged.AddDynamic(this, &ATile::CalculatePopulationGrowthChangeWithNeighbors);
}

void ATile::OnRep_Claimant(ASettlement* NewClaimant)
{
	ClaimantChanged();
}

ASettlement* ATile::GetClaimant()
{
	return Claimant;
}

void ATile::SetClaimant(ASettlement* NewClaimant)
{
	Claimant = NewClaimant;
	ClaimantChanged();
}

void ATile::SetIsRiver(bool IsRiver)
{
	bIsRiver = IsRiver;
	RefreshTileLayout();
	for (ATile* Tile : Neighbors)
	{
		if (Tile) Tile->RefreshTileLayout();
	}
}

void ATile::OnRep_SpawnPointLayout()
{
	OnSpawnPointLayoutChanged();
}

void ATile::RefreshTileLayout()
{
	FTileLayout* NewLayout = FindNewValidTileLayout();
	if (!NewLayout) return;
	TileLayout = *NewLayout;
	// Select random SpawnPointLayout
	SpawnPointLayout = TileLayout.SpawnPointsLayouts[FMath::RandRange(0, TileLayout.SpawnPointsLayouts.Num() - 1)];
	OnSpawnPointLayoutChanged();
	OnTileLayoutChanged();
}

FTileLayout* ATile::FindNewValidTileLayout()
{
	FString ContextString;
	TArray<FTileLayout*> AllRows;
	TileLayouts->GetAllRows<FTileLayout>(ContextString, AllRows);
	TArray<FTileLayout*> PossibleLayouts;
	for (FTileLayout* Row : AllRows)
	{
		if (IsValidTileLayout(Row))
		{
			PossibleLayouts.Add(Row);
		}
	}
	// No valid Layout found, so we use Default
	if (PossibleLayouts.IsEmpty())
	{
		FName DefaultName = "Default";
		return TileLayouts->FindRow<FTileLayout>(DefaultName, ContextString);
	}
	return PossibleLayouts[FMath::RandRange(0, PossibleLayouts.Num() - 1)];
}

bool ATile::IsValidTileLayout(FTileLayout* Layout)
{
	// Tile has River but Row doesn't allow that
	if (bIsRiver && !Layout->AllowRiver) return false;
	// Tile has no Building but Row doesn't allow that
	bool test = !Building;
	bool test2 = !Layout->AllowNoBuilding;
	if (test && test2) return false;
	// Tile has Building but Row doesn't allow that
	if (Building && !Layout->AllowBuilding) return false;
	// Row doesn't allow this biome
	if (!Layout->AllowedBiomes.Contains(Biome)) return false;
	if (bIsRiver)
	{
		// Check if RiverConnections fit
		if (FindRiverConnectionRotation(Layout->RiverConnections) < 0) return false;
	}
	// This layout has no SpawnPointLayout
	if (Layout->SpawnPointsLayouts.IsEmpty()) return false;
	return true;
}

int32 ATile::FindRiverConnectionRotation(TArray<bool> Connections)
{
	if (Connections.Num() != 6) return false;
	TArray<bool> RealConnections;
	for (int32 i = 0; i < 6; ++i)
	{
		RealConnections.Add(Neighbors[i] && Neighbors[i]->bIsRiver);
	}
	UE_LOG(LogTemp, Warning, TEXT("Connections: %d - %d"), Connections.Num(), RealConnections.Num());
	for (int32 Rotation = 0; Rotation < 6; ++Rotation)
	{
		bool ThisRotationWorks = true;
		for (int i = 0; i < 6; ++i)
		{
			if (RealConnections[i] != Connections[(i + Rotation) % 6])
			{
				ThisRotationWorks = false;
				break;
			}
		}
		if (ThisRotationWorks) return Rotation;
	}
	return -1;
}
