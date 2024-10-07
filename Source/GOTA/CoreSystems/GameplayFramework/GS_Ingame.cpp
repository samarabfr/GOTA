// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Ingame.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

//Unreal Engine Mystery Code
void AGS_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Guardians, Params)
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Tribe, Params)
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Colony, Params)

	DOREPLIFETIME(AGS_Ingame, TileMap);

	DOREPLIFETIME(AGS_Ingame, TotalTrees);
	DOREPLIFETIME(AGS_Ingame, TotalForage);
	DOREPLIFETIME(AGS_Ingame, TotalWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxTrees);
	DOREPLIFETIME(AGS_Ingame, IslandMaxWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxForage);

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
	CombatSystem = CreateDefaultSubobject<UCombatSystem>(TEXT("Combat System"));
	StartParameter = CreateDefaultSubobject<UStartParameter>(TEXT("Start Parameter"));

	// 4 because max players, but this should be a constant somewhere
	Guardians.SetNumZeroed(4);
}

void AGS_Ingame::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AddReplicatedSubObject(TotalTrees);
		AddReplicatedSubObject(TotalForage);
		AddReplicatedSubObject(TotalWildlife);
		AddReplicatedSubObject(CombatSystem);
		AddReplicatedSubObject(StartParameter);
	}
	// Spawn Static Mesh Batcher
	StaticMeshBatcher = GetWorld()->SpawnActor<AStaticMeshBatcher>();
}

void AGS_Ingame::SetTribe(ATribe* NewTribe)
{
	Tribe = NewTribe;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Tribe, this)
}

void AGS_Ingame::SetColony(AColony* NewColony)
{
	Colony = NewColony;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Colony, this)
}

void AGS_Ingame::GuardiansChanged()
{
	OnGuardiansChanged.Broadcast(this);
}

TArray<AGuardian*> AGS_Ingame::GetGuardians() const
{
	return Guardians;
}

AGuardian* AGS_Ingame::GetGuardian(const int32 GOTAPlayerID) const
{
	if (Guardians.IsValidIndex(GOTAPlayerID))
		return Guardians[GOTAPlayerID];
	return nullptr;
}

void AGS_Ingame::SetGuardian(const int32 GOTAPlayerID, AGuardian* Guardian)
{
	if (Guardians.IsValidIndex(GOTAPlayerID))
	{
		Guardians[GOTAPlayerID] = Guardian;
		MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Guardians, this)
		GuardiansChanged();
	}
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
