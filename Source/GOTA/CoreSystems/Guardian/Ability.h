// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTarget.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

class UAbilityWidget;
class UAbilitySettings;

UCLASS()
class GOTA_API AAbility : public AActor
{
	GENERATED_BODY()

	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	AAbility();

	void S_Init(UAbilitySettings* InSettings, const FName InAbilitySlotName);

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<UAbilitySettings> Settings;

public:
	UAbilitySettings* GetSettings() { return Settings.Get(); }

	// ---------------------------------------- Activation ----------------------------------------

	UFUNCTION(Server, Reliable)
	void SRPC_ActivateAbility(FAbilityTarget Target);

protected:
	virtual void S_ActivateAbility(FAbilityTarget Target);

public:
	void ActivateAbility(FAbilityTarget Target);

	virtual bool IsValidTarget(FAbilityTarget Target) { return false; }

	// ---------------------------------------- UI ----------------------------------------

private:	
	TWeakObjectPtr<UAbilityWidget> Widget;
	
	UPROPERTY(ReplicatedUsing = OnRep_SlotName)
	FName SlotName;
	
	UFUNCTION()
	void OnRep_SlotName(FName OldSlotName);
	
	UFUNCTION(Server, Reliable)
	void SRPC_SetSlotName(FName NewSlotName);

	// Only Notifies UI if this Ability is owned by the local playercontroller
	void NotifySlotNameChangeToUI(FName OldSlotName, FName NewSlotName);
	
public:
	void SetSlotName(const FName NewSlotName);
	FName GetSlotName() const { return SlotName; }
};
