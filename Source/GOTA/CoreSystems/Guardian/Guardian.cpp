// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian.h"

#include "Ability.h"
#include "AbilityManager.h"
#include "AbilitySettings.h"
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

	UAbilityManager* AbilityManager = GetGameInstance()->GetSubsystem<UAbilityManager>();
	for (UAbilitySettings* AbilitySettings : AbilityManager->GetAllAbilities())
	{
		AActor* Actor = GetWorld()->SpawnActor(AbilitySettings->GetAbilityClass());
		AAbility* Ability = Cast<AAbility>(Actor);
		AbilityBar.Add(Ability);
	}
}

void AGuardian::S_Init(UGuardianSettings* InSettings)
{
	Settings = InSettings;
}

// ---------------------------------------- Utility ----------------------------------------

// ---------------------------------------- Abilities ----------------------------------------

void AGuardian::ActivateAbility(int32 Index)
{
	int32 ShiftedIndex = Index - 1;
	if (AbilityBar.IsValidIndex(ShiftedIndex) && AbilityBar[ShiftedIndex])
	{
		AbilityBar[ShiftedIndex]->ActivateAbility();
	}
}
