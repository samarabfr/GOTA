// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/GameplayFramework/GM_Ingame.h"

#include "GM_GOTARL_Ingame.generated.h"


class AGS_GotaRL_Ingame;
class AGuardianSimulator;
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
	
	TWeakObjectPtr<AGS_GotaRL_Ingame> GotaRLGameState;

	virtual void LoadGame() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGuardian> GuardianClass;

	virtual void BeginPlay() override;
};
