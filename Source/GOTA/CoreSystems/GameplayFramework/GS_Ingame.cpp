// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Ingame.h"

#include "CombatSystem.h"
#include "GameSettings.h"
#include "StartParameter.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttribute.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "GOTA/CoreSystems/Utility/StaticMeshBatcher.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------- Replication Setup -------------------

void AGS_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Guardians, Params)
	
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, TileMap, Params)
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, GameSettings, Params)
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Colony, Params)
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Tribe, Params)


	DOREPLIFETIME(AGS_Ingame, TotalTrees);
	DOREPLIFETIME(AGS_Ingame, TotalForage);
	DOREPLIFETIME(AGS_Ingame, TotalWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxTrees);
	DOREPLIFETIME(AGS_Ingame, IslandMaxWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxForage);

	DOREPLIFETIME(AGS_Ingame, CombatSystem);
	DOREPLIFETIME(AGS_Ingame, StartParameter);
}

void AGS_Ingame::AddReplicatedSubobjects()
{
	AddReplicatedSubObject(TotalTrees);
	AddReplicatedSubObject(TotalForage);
	AddReplicatedSubObject(TotalWildlife);
	AddReplicatedSubObject(CombatSystem);
	AddReplicatedSubObject(StartParameter);
}

// ------------------- LifeCycle -------------------

AGS_Ingame::AGS_Ingame()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.0f;

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
		S_Init();
	else
		C_Init();
}

void AGS_Ingame::S_Init()
{
	AddReplicatedSubobjects();
	SpawnGameSettingsActor();
	SpawnStaticMeshBatcher();
}

void AGS_Ingame::C_Init()
{
	SpawnStaticMeshBatcher();
}

// ------------------- Utility -------------------

// ------------------- TileMap -------------------

void AGS_Ingame::SetTileMap(ATileMap* NewTileMap)
{
	TileMap = NewTileMap;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, TileMap, this)
}

// ------------------- GameSettings -------------------

void AGS_Ingame::SpawnGameSettingsActor()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName("GameSettings");
	GameSettings = GetWorld()->SpawnActor<AGameSettings>(SpawnParams);
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, GameSettings, this)
}

// ------------------- StaticMeshBatcher -------------------

void AGS_Ingame::SpawnStaticMeshBatcher()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName("StaticMeshBatcher");
	StaticMeshBatcher = GetWorld()->SpawnActor<AStaticMeshBatcher>();
}

// ------------------- Settlements -------------------

void AGS_Ingame::SetColony(AColony* NewColony)
{
	Colony = NewColony;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Colony, this)
}

void AGS_Ingame::SetTribe(ATribe* NewTribe)
{
	Tribe = NewTribe;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Tribe, this)
}

// ------------------- Guardians -------------------

void AGS_Ingame::GuardiansChanged()
{
	OnGuardiansChanged.Broadcast(this);
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

// ------------------- Entities -------------------

// ------------------- Island Health -------------------

void AGS_Ingame::RegisterTileForTotalsUpdates(ATile* Tile)
{
}

void AGS_Ingame::CountIslandMaxEcoValues()
{
	GetTileMap()->CountAllMaxEcoValues(IslandMaxTrees, IslandMaxWildlife, IslandMaxForage);
}

// ------------------- Game Ending -------------------

void AGS_Ingame::S_EndGame_Implementation(::EGameEnding Ending, const FString& EndingMessage)
{
	if (GameEnded) return;
	GameEnded = true;
	OnGameEnding.Broadcast(Ending, EndingMessage);
}
