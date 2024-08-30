// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Ingame.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void AGS_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGS_Ingame, TileMap);

	DOREPLIFETIME(AGS_Ingame, TotalTrees);
	DOREPLIFETIME(AGS_Ingame, TotalForage);
	DOREPLIFETIME(AGS_Ingame, TotalWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxTrees);
	DOREPLIFETIME(AGS_Ingame, IslandMaxWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxForage);

	DOREPLIFETIME(AGS_Ingame, TotalColonialPopulation);
	DOREPLIFETIME(AGS_Ingame, TotalNativePopulation);

	DOREPLIFETIME(AGS_Ingame, MaxTurnTime);
	DOREPLIFETIME(AGS_Ingame, IsCalculatingTurn);
	DOREPLIFETIME(AGS_Ingame, ShouldTickTurnTime);
	DOREPLIFETIME(AGS_Ingame, TurnCounter);

	DOREPLIFETIME(AGS_Ingame, CombatSystem);
	DOREPLIFETIME(AGS_Ingame, StartParameter);
}

AGS_Ingame::AGS_Ingame()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	TotalTrees = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Trees"));
	TotalForage = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Forage"));
	TotalWildlife = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Wildlife"));
	TotalColonialPopulation = CreateDefaultSubobject<UPopulationSummary>(TEXT("Total Colonial Population"));
	TotalNativePopulation = CreateDefaultSubobject<UPopulationSummary>(TEXT("Total Native Population"));
	CombatSystem = CreateDefaultSubobject<UCombatSystem>(TEXT("Combat System"));
	StartParameter = CreateDefaultSubobject<UStartParameter>(TEXT("Start Parameter"));
}

void AGS_Ingame::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		AddReplicatedSubObject(TotalTrees);
		AddReplicatedSubObject(TotalForage);
		AddReplicatedSubObject(TotalWildlife);
		AddReplicatedSubObject(TotalColonialPopulation);
		AddReplicatedSubObject(TotalNativePopulation);
		AddReplicatedSubObject(CombatSystem);
		AddReplicatedSubObject(StartParameter);
	}
	// Spawn Static Mesh Batcher
	StaticMeshBatcher = GetWorld()->SpawnActor<AStaticMeshBatcher>();
}

void AGS_Ingame::Tick(float DeltaSeconds)
{
	if (!ShouldTickTurnTime) return;
	SetElapsedTurnTime(FMath::Min(ElapsedTurnTime + DeltaSeconds, MaxTurnTime));
}


void AGS_Ingame::IncreaseTurnCounter()
{
	++TurnCounter;
	OnTurnCounterChanged.Broadcast();
}

void AGS_Ingame::OnRep_TurnCounter()
{
	OnTurnCounterChanged.Broadcast();
}

void AGS_Ingame::SetElapsedTurnTime(float NewValue)
{
	ElapsedTurnTime = NewValue;
	OnTurnTimerChanged.Broadcast(ElapsedTurnTime, MaxTurnTime);
}

void AGS_Ingame::SetElapsedTurnTimeMulticast_Implementation(float NewValue)
{
	SetElapsedTurnTime(NewValue);
}


void AGS_Ingame::NextTurn()
{
	SetElapsedTurnTimeMulticast(MaxTurnTime);
}

void AGS_Ingame::TurnCalculationStart()
{
	ShouldTickTurnTime = false;
	IsCalculatingTurn = true;
	OnTurnCalculationStart.Broadcast();
}

void AGS_Ingame::TurnCalculationEnd()
{
	ShouldTickTurnTime = true;
	IsCalculatingTurn = false;
	OnTurnCalculationEnd.Broadcast();
}


void AGS_Ingame::TogglePause()
{
	ShouldTickTurnTime = !ShouldTickTurnTime;
}

void AGS_Ingame::RegisterColonialSettlementForTotalsUpdates(UPopulationSummary* Population)
{
	TotalColonialPopulation->RegisterPopulationSummary(Population);
}

void AGS_Ingame::RegisterNativeSettlementForTotalsUpdates(UPopulationSummary* Population)
{
	TotalNativePopulation->RegisterPopulationSummary(Population);
}

void AGS_Ingame::RegisterPopConForTotals(UPopulationContainer* PopCon, EAffiliation Affiliation)
{
	if (Affiliation == EAffiliation::Ally)
		TotalNativePopulation->RegisterPopulationContainer(PopCon);
	if (Affiliation == EAffiliation::Enemy)
		TotalColonialPopulation->RegisterPopulationContainer(PopCon);
}

void AGS_Ingame::UnregisterPopConForTotals(UPopulationContainer* PopCon, EAffiliation Affiliation)
{
	if (Affiliation == EAffiliation::Ally)
		TotalNativePopulation->UnregisterPopulationContainer(PopCon);
	if (Affiliation == EAffiliation::Enemy)
		TotalColonialPopulation->UnregisterPopulationContainer(PopCon);
}

void AGS_Ingame::EndGame_Implementation(::EGameEnding Ending, const FString& EndingMessage)
{
	if (GameEnded) return;
	GameEnded = true;
	OnGameEnding.Broadcast(Ending, EndingMessage);
}

void AGS_Ingame::CountIslandMaxEcoValues()
{
	TileMap->CountAllMaxEcoValues(IslandMaxTrees, IslandMaxWildlife, IslandMaxForage);
}
