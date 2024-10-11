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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	UGuardianSettings* Settings;

public:
	void Init(UGuardianSettings* InSettings);

	UGuardianSettings* GetSettings() const { return Settings; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category="PlayerController")
	void SetupGAM(AMouseUtils* MouseUtils_);
};
