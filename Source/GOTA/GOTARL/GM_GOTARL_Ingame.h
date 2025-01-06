// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/GameplayFramework/GM_Ingame.h"

#include "GM_GOTARL_Ingame.generated.h"


class ASimulatedGuardian;
class ASimulatedGuardianManager;
class UGuardianSettings;

UCLASS(Blueprintable)
class GOTA_API AGM_GOTARL_Ingame : public AGM_Ingame
{
	GENERATED_BODY()
	AGM_GOTARL_Ingame();

	virtual void CreateGuardians() override;

	UPROPERTY()
	UGuardianSettings* GuardianSettings;

	TWeakObjectPtr<ASimulatedGuardianManager> LearningManager;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASimulatedGuardian> SimulatedGuardianClass;

	virtual void BeginPlay() override;
};
