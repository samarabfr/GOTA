// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Ingame.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void AGS_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_Ingame, MaxTurnTime);
	DOREPLIFETIME(AGS_Ingame, IsCalculatingTurn);
	DOREPLIFETIME(AGS_Ingame, ShouldTickTurnTime);

	DOREPLIFETIME(AGS_Ingame, TileMap);
	DOREPLIFETIME(AGS_Ingame, Settlements);
	DOREPLIFETIME(AGS_Ingame, Guardians);
	DOREPLIFETIME(AGS_Ingame, TileEntities);
	
	DOREPLIFETIME(AGS_Ingame, TotalTrees);
	DOREPLIFETIME(AGS_Ingame, TotalForage);
	DOREPLIFETIME(AGS_Ingame, TotalWildlife);

	DOREPLIFETIME(AGS_Ingame, TotalColonialPopulation);
	DOREPLIFETIME(AGS_Ingame, TotalNativePopulation);
	DOREPLIFETIME(AGS_Ingame, TotalPopulation);
}

AGS_Ingame::AGS_Ingame()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	TotalTrees = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Trees"));
	TotalForage = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Forage"));
	TotalWildlife = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Wildlife"));
	TotalColonialPopulation = CreateDefaultSubobject<UPopulationSummary>(TEXT("Total Colonial Population"));
	TotalNativePopulation = CreateDefaultSubobject<UPopulationSummary>(TEXT("Total Native Population"));
	TotalPopulation = CreateDefaultSubobject<UPopulationSummary>(TEXT("Total Population"));
}

void AGS_Ingame::AddTileEntity(AEntity* NewTileEntity)
{
	TileEntities.Add(NewTileEntity);
}

float AGS_Ingame::GetElapsedTurnTime()
{
	return ElapsedTurnTime;
}

void AGS_Ingame::SetElapsedTurnTime(float NewValue)
{
	ElapsedTurnTime = NewValue;
	TurnTimerChanged.Broadcast(ElapsedTurnTime, MaxTurnTime);
}

void AGS_Ingame::MulticastSetElapsedTurnTime_Implementation(float NewValue)
{
	SetElapsedTurnTime(NewValue);
}

void AGS_Ingame::CallCalculationStart()
{
	TurnCalculationStart.Broadcast();
}

void AGS_Ingame::CallCalculationEnd()
{
	TurnCalculationEnd.Broadcast();
}

void AGS_Ingame::Init()
{
	AddReplicatedSubObject(TotalTrees);
	AddReplicatedSubObject(TotalForage);
	AddReplicatedSubObject(TotalWildlife);
	AddReplicatedSubObject(TotalColonialPopulation);
	AddReplicatedSubObject(TotalNativePopulation);
	AddReplicatedSubObject(TotalPopulation);
	SpawnStaticMeshBatcher();
}