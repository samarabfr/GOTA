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
	DOREPLIFETIME(ATile, Forage);
	DOREPLIFETIME(ATile, Wildlife);
	DOREPLIFETIME(ATile, Building);
	DOREPLIFETIME(ATile, TreeGrowth);
	DOREPLIFETIME(ATile, Neighbors);
	DOREPLIFETIME(ATile, TreeGrowthChange);
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
	Forage = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Forage"));
	Wildlife = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Wildlife"));
	TreeGrowth = CreateDefaultSubobject<UGOTAAttribute>(TEXT("TreeGrowth"));
	TreeGrowthChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("TreeGrowthChange"));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the GameState
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();

	if(HasAuthority())
	{
		AddReplicatedSubObject(Trees);
		AddReplicatedSubObject(Forage);
		AddReplicatedSubObject(Wildlife);
		AddReplicatedSubObject(TreeGrowth);
		AddReplicatedSubObject(TreeGrowthChange);
	}
}

void ATile::Init()
{
	Trees->OnChanged.AddDynamic(this, &ATile::CalculateTreeGrowthChangeWithNeighbors);
	Trees->SetMaximum(BalanceData->MaxTrees);
	Trees->SetCurrent(BalanceData->StartingTrees);
	Forage->SetMaximum(BalanceData->MaxForage);
	Forage->SetCurrent(BalanceData->StartingForage);
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
	if(TreeGrowth->Current > BalanceData->TreeGrowthThreshold)
	{
		const int32 TreeGrowCount = TreeGrowth->Current/BalanceData->TreeGrowthThreshold;
		Trees->Add(TreeGrowCount, _);
		TreeGrowth->Subtract(TreeGrowCount * BalanceData->TreeGrowthThreshold, _);
	}
}

void ATile::CalculateTreeGrowthChange()
{
	TreeGrowthChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if(Neighbors[i])
		{
			TreeGrowthChange->Add(Neighbors[i]->Trees->Current, _);
		}
	}
	TreeGrowthChange->Add(Trees->Current, _);
}

void ATile::CalculateTreeGrowthChangeWithNeighbors(int32 Change)
{
	if(bFreezeGrowthChanges) return;
	
	CalculateTreeGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if(Neighbors[i]) Neighbors[i]->CalculateTreeGrowthChange();
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