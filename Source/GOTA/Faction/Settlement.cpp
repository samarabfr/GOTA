// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASettlement, ClaimColor);
	DOREPLIFETIME(ASettlement, JobManager);
	
	DOREPLIFETIME(ASettlement, Population);
	DOREPLIFETIME(ASettlement, Food);
	DOREPLIFETIME(ASettlement, Wood);
	DOREPLIFETIME(ASettlement, Expansion);
}

ASettlement::ASettlement()
{
	// Replication stuff
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("FoodAttribute"));
	Wood = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WoodAttribute"));
	Population = CreateDefaultSubobject<UPopulation>(TEXT("PopulationAttribute"));
	Expansion = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("ExpansionAttribute"));
	JobManager = CreateDefaultSubobject<UJobManager>(TEXT("JobManager"));


}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the GameState
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();

	if(HasAuthority())
	{
		AddReplicatedSubObject(Food);
		AddReplicatedSubObject(Wood);
		AddReplicatedSubObject(Population);
		AddReplicatedSubObject(Expansion);
		AddReplicatedSubObject(JobManager);

		JobManager->BindToPopulationAttribute(Population);
	}
}

void ASettlement::OnBuildingAdded(UBuilding* Building)
{
	JobManager->BindToBuilding(Building);
}

void ASettlement::OnBuildingRemoved(UBuilding* Building)
{
	JobManager->UnbindToBuilding(Building);
}
