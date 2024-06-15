// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASettlement, ClaimColor);
	DOREPLIFETIME(ASettlement, Population);
	DOREPLIFETIME(ASettlement, Food);
	DOREPLIFETIME(ASettlement, Wood);
	DOREPLIFETIME(ASettlement, Expansion);
}

ASettlement::ASettlement()
{
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("FoodAttribute"));
	Wood = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WoodAttribute"));
	Population = CreateDefaultSubobject<UGOTAAttributePopulation>(TEXT("PopulationAttribute"));
	Expansion = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("ExpansionAttribute"));
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the GameState
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();
}