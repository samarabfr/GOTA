// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability.h"

#include "AbilitySettings.h"
#include "AbilitySlotRegister.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void AAbility::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(AAbility, Settings, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AAbility, SlotName, Params);
}

void AAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	NotifySlotNameChangeToUI(SlotName, FName());
}

// ---------------------------------------- Lifecycle ----------------------------------------

AAbility::AAbility()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = false;
	SetNetUpdateFrequency(1.0f);

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AAbility::S_Init(UAbilitySettings* InSettings, const FName InAbilitySlotName)
{
	Settings = InSettings;
	SetSlotName(InAbilitySlotName);
}

// ---------------------------------------- Utility ----------------------------------------

// ---------------------------------------- Activation ----------------------------------------

void AAbility::S_ActivateAbility(FAbilityTarget Target)
{
}

void AAbility::SRPC_ActivateAbility_Implementation(FAbilityTarget Target)
{
	S_ActivateAbility(Target);
}

void AAbility::ActivateAbility(FAbilityTarget Target)
{
	SRPC_ActivateAbility(Target);
}

// ---------------------------------------- Ability Slot ----------------------------------------

void AAbility::OnRep_SlotName(const FName OldSlotName)
{
	NotifySlotNameChangeToUI(OldSlotName, SlotName);
}

void AAbility::SRPC_SetSlotName_Implementation(const FName NewSlotName)
{
	SlotName = NewSlotName;
	MARK_PROPERTY_DIRTY_FROM_NAME(AAbility, SlotName, this)
}

void AAbility::NotifySlotNameChangeToUI(const FName OldSlotName, const FName NewSlotName)
{
	// Make sure this is only done on the local PlayerController
	if (!IsOwnedBy(GetWorld()->GetFirstPlayerController()))
		return;
	
	if (UAbilitySlotRegister* AbilitySlotRegister = GetGameInstance()->GetSubsystem<UAbilitySlotRegister>())
	{
		AbilitySlotRegister->UnassignAbilityFromSlot(this, OldSlotName);
		AbilitySlotRegister->AssignAbilityToSlot(this, NewSlotName);
	}
}

void AAbility::SetSlotName(const FName NewSlotName)
{
	if (NewSlotName == SlotName) return;
	NotifySlotNameChangeToUI(SlotName, NewSlotName);
	SRPC_SetSlotName(NewSlotName);
}
