// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "Net/UnrealNetwork.h"

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
	}
}

//====================================================================
//--------------------Claimant
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::CalculateTurn()
{
	// calculate Tree growth
	
	// grow trees
	if(TreeGrowth->Current > DataAsset->TreeGrowthThreshold)
	{
		int32 _ = 0;
		int32 TreeGrowCount = TreeGrowth->Current/DataAsset->TreeGrowthThreshold;
		Trees->Add(TreeGrowCount, _);
		TreeGrowth->Subtract(TreeGrowCount * DataAsset->TreeGrowthThreshold, _);
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