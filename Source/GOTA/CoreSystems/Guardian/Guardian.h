// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Guardian.generated.h"

class UAbilitySettings;
class APC_Ingame;
class AAbility;
class UGuardianSettings;

UCLASS()
class GOTA_API AGuardian : public ACharacter
{
	GENERATED_BODY()

	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	AGuardian();

	void S_Init(UGuardianSettings* InSettings);

	virtual void BeginPlay() override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<UGuardianSettings> Settings;
	
public:
	UGuardianSettings* GetSettings() const { return Settings.Get(); }

	// ---------------------------------------- Abilities ----------------------------------------
private:
	UPROPERTY(Replicated)
	TArray<TWeakObjectPtr<AAbility>> Abilities;

	UFUNCTION(Server, Reliable)
	void SRPC_LearnAbility(UAbilitySettings* AbilitySettings, FName AbilitySlotName);
	
public:
	void LearnAbility(UAbilitySettings* AbilitySettings, FName AbilitySlotName);
};
