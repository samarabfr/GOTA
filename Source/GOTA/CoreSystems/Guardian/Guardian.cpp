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
	DOREPLIFETIME_WITH_PARAMS(AGuardian, AbilityBar, Params);
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

	AbilityBar.SetNumZeroed(8);
}

void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->IncrementReplicationCount();

	const UAbilityManager* AbilityManager = GetGameInstance()->GetSubsystem<UAbilityManager>();
	for (UAbilitySettings* AbilitySettings : AbilityManager->GetAllAbilities())
	{
		S_LearnAbility(AbilitySettings);
	}
}

void AGuardian::S_Init(UGuardianSettings* InSettings)
{
	Settings = InSettings;
}

// ---------------------------------------- Utility ----------------------------------------

// ---------------------------------------- Abilities ----------------------------------------

void AGuardian::S_LearnAbility(const UAbilitySettings* AbilitySettings)
{
	FActorSpawnParameters AbilitySpawnParams;
	AbilitySpawnParams.Owner = this;
	AActor* Actor = GetWorld()->SpawnActor(AbilitySettings->GetAbilityClass(),
	                                       &FTransform::Identity,
	                                       AbilitySpawnParams);
	AAbility* Ability = Cast<AAbility>(Actor);
	Abilities.Add(Ability);
	MARK_PROPERTY_DIRTY_FROM_NAME(AGuardian, Abilities, this)
	for (int i = 0; i < AbilityBar.Num(); ++i)
	{
		if (!AbilityBar[i].IsValid())
		{
			AbilityBar[i] = Ability;
			MARK_PROPERTY_DIRTY_FROM_NAME(AGuardian, AbilityBar, this)
			break;
		}
	}
}

AAbility* AGuardian::GetAbilityInSlot(int32 Index)
{
	return AbilityBar.IsValidIndex(Index) ? AbilityBar[Index].Get() : nullptr;
}
