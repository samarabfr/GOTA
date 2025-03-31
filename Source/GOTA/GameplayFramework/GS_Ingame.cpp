// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Ingame.h"

#include "GOTA/Utility/LoadingManager.h"
#include "StartParameter.h"
#include "GOTA/Utility/GOTAAttribute.h"
#include "GOTA/Guardian/Guardian.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tilemap/TileMap.h"
#include "GOTA/Utility/StaticMeshBatcher.h"
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
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Colony, Params)
	DOREPLIFETIME_WITH_PARAMS(AGS_Ingame, Tribe, Params)


	DOREPLIFETIME(AGS_Ingame, TotalTrees);
	DOREPLIFETIME(AGS_Ingame, TotalForage);
	DOREPLIFETIME(AGS_Ingame, IslandMaxTrees);
	DOREPLIFETIME(AGS_Ingame, IslandMaxForage);

	DOREPLIFETIME(AGS_Ingame, StartParameter);
}

void AGS_Ingame::AddReplicatedSubobjects()
{
	AddReplicatedSubObject(TotalTrees);
	AddReplicatedSubObject(TotalForage);
	AddReplicatedSubObject(StartParameter);
}

// ------------------- LifeCycle -------------------

AGS_Ingame::AGS_Ingame()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	SetNetUpdateFrequency(1.0f);

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.0f;

	TotalTrees = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Trees"));
	TotalForage = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Forage"));
	StartParameter = CreateDefaultSubobject<UStartParameter>(TEXT("Start Parameter"));

	// 4 because max players, but this should be a constant somewhere
	Guardians.SetNumZeroed(4);
	GuardianAIControllers.SetNumZeroed(4);
}

void AGS_Ingame::DeleteEverything()
{
	if (StaticMeshBatcher) StaticMeshBatcher->Clear();
	if (TileMap) TileMap->Delete();
	if (LoadingManager) LoadingManager->Delete();
	if (Colony) Colony->Delete();
	if (Tribe) Tribe->Delete();
	for (AGuardian* Guardian : Guardians)
	{
		if (Guardian) Guardian->Delete();
	}
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

// ------------------- LoadingManager -------------------

void AGS_Ingame::SetLoadingManager(ALoadingManager* NewLoadingManager)
{
	LoadingManager = NewLoadingManager;
}

void AGS_Ingame::IncrementReplicationCount()
{
	LoadingManager->IncrementReplicationCount();
}

// ------------------- StaticMeshBatcher -------------------

void AGS_Ingame::SpawnStaticMeshBatcher()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName("StaticMeshBatcher");
	StaticMeshBatcher = GetWorld()->SpawnActor<AStaticMeshBatcher>();
}

// ------------------- Settlements -------------------

void AGS_Ingame::SetColony(ASettlement* NewColony)
{
	Colony = NewColony;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Colony, this)
}

void AGS_Ingame::SetTribe(ASettlement* NewTribe)
{
	Tribe = NewTribe;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGS_Ingame, Tribe, this)
}

// ------------------- Guardians -------------------

void AGS_Ingame::GuardiansChanged()
{
	OnGuardiansChanged.Broadcast();
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
	GetTileMap()->CountAllMaxEcoValues(IslandMaxTrees, IslandMaxForage);
}

// ------------------- Game Ending -------------------

void AGS_Ingame::S_EndGame_Implementation(::EGameEnding Ending, const FString& EndingMessage)
{
	if (GameEnded) return;
	GameEnded = true;
	GameEnding = Ending;
	OnGameEnding.Broadcast(Ending, EndingMessage);
}

// ------------------- Reinforcement Learning Manager -------------------

TArray<AAIController*> AGS_Ingame::GetGuardianAIControllers() const
{
	return GuardianAIControllers;
}

AAIController* AGS_Ingame::GetGuardianAIController(int32 GOTAPlayerID) const
{
	if (!Guardians.IsValidIndex(GOTAPlayerID)) return nullptr;
	return GuardianAIControllers[GOTAPlayerID];
}

void AGS_Ingame::SetGuardianAIController(int32 GOTAPlayerID, AAIController* GuardianAIController) 
{
	if (!Guardians.IsValidIndex(GOTAPlayerID)) return;
	GuardianAIControllers[GOTAPlayerID] = GuardianAIController;
}

void AGS_Ingame::S_AddManager(TSubclassOf<AActor> ManagerClass, AActor* Manager)
{
	if (RL_Managers.Contains(ManagerClass)) return;
	RL_Managers.Add(ManagerClass, Manager);
}
