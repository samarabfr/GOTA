// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian.h"

#include "Ability.h"
#include "AbilityManager.h"
#include "AbilitySettings.h"
#include "GuardianSettings.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"

// ------------------------------------ Replication Setup --------------------------------------

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
	DOREPLIFETIME_WITH_PARAMS(AGuardian, Abilities, Params);
}

// ---------------------------------------- Lifecycle ----------------------------------------

AGuardian::AGuardian()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.2f;
}

void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->IncrementReplicationCount();
}

void AGuardian::S_Init(UGuardianSettings* InSettings)
{
	Settings = InSettings;
}

// ---------------------------------------- Utility ----------------------------------------

// ---------------------------------------- Abilities ----------------------------------------

// ToDo implement replication shit, srpc and that stuff to learn abilities... react to replication of a new ability as well

void AGuardian::S_LearnAbility(UAbilitySettings* AbilitySettings, FName AbilitySlotName)
{
	FActorSpawnParameters AbilitySpawnParams;
	AbilitySpawnParams.Owner = this;
	AActor* Actor = GetWorld()->SpawnActor(AbilitySettings->GetAbilityClass(),
	                                       &FTransform::Identity,
	                                       AbilitySpawnParams);
	AAbility* Ability = Cast<AAbility>(Actor);
	Ability->S_Init(AbilitySettings, AbilitySlotName);
	
	Abilities.Add(Ability);
	MARK_PROPERTY_DIRTY_FROM_NAME(AGuardian, Abilities, this)
}
