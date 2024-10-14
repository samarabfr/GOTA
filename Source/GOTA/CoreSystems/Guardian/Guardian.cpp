// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"

void AGuardian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(AGuardian, Settings, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

AGuardian::AGuardian()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = false;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5f;
}

void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	// IncreaseReplicationCount for LoadingProcess
	if (GetWorld()->GetGameState<AGS_Ingame>()->LoadingManager)
		GetWorld()->GetGameState<AGS_Ingame>()->LoadingManager->IncrementReplicationCount();
}

void AGuardian::Init(UGuardianSettings* InSettings)
{
	Settings = InSettings;
}

