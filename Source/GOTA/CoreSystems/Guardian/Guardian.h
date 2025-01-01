// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GOTA/CoreSystems/Utility/MouseUtils.h"
#include "Guardian.generated.h"

class UGuardianSettings;
class UGuardianDataAsset;

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

public:
	UGuardianSettings* GetSettings() const { return Settings; }
	
	// ---------------------------------------- Abilities ----------------------------------------

	
};
