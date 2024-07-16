// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameState.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void AGOTAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGOTAGameState, MaxTurnTime);
	DOREPLIFETIME(AGOTAGameState, IsCalculatingTurn);
	DOREPLIFETIME(AGOTAGameState, ShouldTickTurnTime);

	DOREPLIFETIME(AGOTAGameState, TileMap);
	DOREPLIFETIME(AGOTAGameState, Factions);
	DOREPLIFETIME(AGOTAGameState, Settlements);
	DOREPLIFETIME(AGOTAGameState, Guardians);
	DOREPLIFETIME(AGOTAGameState, TileEntities);
	
	DOREPLIFETIME(AGOTAGameState, TotalTrees);
	DOREPLIFETIME(AGOTAGameState, TotalForage);
	DOREPLIFETIME(AGOTAGameState, TotalWildlife);

	DOREPLIFETIME(AGOTAGameState, TotalColonialPopulation);
	DOREPLIFETIME(AGOTAGameState, TotalNativePopulation);
	DOREPLIFETIME(AGOTAGameState, TotalPopulation);
}

AGOTAGameState::AGOTAGameState()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	TotalTrees = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Trees"));
	TotalForage = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Forage"));
	TotalWildlife = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Wildlife"));
	TotalColonialPopulation = CreateDefaultSubobject<UPopulation>(TEXT("Total Colonial Population"));
	TotalNativePopulation = CreateDefaultSubobject<UPopulation>(TEXT("Total Native Population"));
	TotalPopulation = CreateDefaultSubobject<UPopulation>(TEXT("Total Population"));
}

void AGOTAGameState::AddFaction(AFaction* NewFaction)
{
	Factions.Add(NewFaction);
}

void AGOTAGameState::AddTileEntity(ATileEntity* NewTileEntity)
{
	TileEntities.Add(NewTileEntity);
}

float AGOTAGameState::GetElapsedTurnTime()
{
	return ElapsedTurnTime;
}

void AGOTAGameState::SetElapsedTurnTime(float NewValue)
{
	ElapsedTurnTime = NewValue;
	TurnTimerChanged.Broadcast(ElapsedTurnTime, MaxTurnTime);
}

void AGOTAGameState::MulticastSetElapsedTurnTime_Implementation(float NewValue)
{
	SetElapsedTurnTime(NewValue);
}

void AGOTAGameState::CallCalculationStart()
{
	TurnCalculationStart.Broadcast();
}

void AGOTAGameState::CallCalculationEnd()
{
	TurnCalculationEnd.Broadcast();
}

void AGOTAGameState::Init()
{
	AddReplicatedSubObject(TotalTrees);
	AddReplicatedSubObject(TotalForage);
	AddReplicatedSubObject(TotalWildlife);
	AddReplicatedSubObject(TotalColonialPopulation);
	AddReplicatedSubObject(TotalNativePopulation);
	AddReplicatedSubObject(TotalPopulation);
}