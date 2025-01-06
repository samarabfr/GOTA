// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Guardian.generated.h"

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
	UGuardianSettings* Settings;

	UPROPERTY(Replicated)
	TWeakObjectPtr<APC_Ingame> PlayerController;

public:
	UGuardianSettings* GetSettings() const { return Settings; }
	void SetPlayerController(APC_Ingame* NewPlayerController);

	// ---------------------------------------- Abilities ----------------------------------------
private:
	UPROPERTY()
	TArray<TWeakObjectPtr<AAbility>> AbilityBar;

	UPROPERTY()
	TWeakObjectPtr<AAbility> CurrentlyTargeting;

	void LearnAbility(AAbility* Ability);

	void StartTargeting(AAbility* Ability);

public:
	void ActivateAbility(int32 Index);

	bool IsCurrentlyTargeting() { return CurrentlyTargeting != nullptr; }

	void CancelTargeting();
};
