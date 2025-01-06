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

void AGuardian::SetPlayerController(APC_Ingame* NewPlayerController)
{
	PlayerController = NewPlayerController;
}

// ---------------------------------------- Abilities ----------------------------------------

void AGuardian::LearnAbility(AAbility* Ability)
{
}

void AGuardian::StartTargeting(AAbility* Ability)
{
	CurrentlyTargeting = Ability;
}

void AGuardian::ActivateAbility(int32 Index)
{
	AAbility* Ability = nullptr;
	if (AbilityBar.IsValidIndex(Index - 1))
	{
		Ability = AbilityBar[Index - 1].Get();
	}

	// Either Input was invalid or there is no skill in the selected index, either way we tried to activate
	// an ability so we should probably cancel any active targeting process
	if (!Ability)
	{
		CancelTargeting();
		return;
	}

	if (CurrentlyTargeting == Ability)
	{
		Ability->ActivateAbility();
		CancelTargeting();
	}
	else
	{
		CancelTargeting();
		StartTargeting(Ability);
	}
}

void AGuardian::CancelTargeting()
{
	CurrentlyTargeting = nullptr;
}
