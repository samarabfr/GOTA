// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "GOTA/Faction/Building.h"
#include "Net/UnrealNetwork.h"

bool ATile::bFreezeGrowthChanges = false;

//Unreal Engine Mystery Code
void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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
}

// Constructor
ATile::ATile()
{
	IsWalkable = true;
	IsClaimable = true;

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

void ATile::AddBuildingToReplication()
{
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->Population);
	AddReplicatedSubObject(Building->Production);
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
