// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/GameplayFramework/GM_Ingame.h"

#include "GM_SelfPlay.generated.h"


class AGS_SelfPlay;
class AGuardianSimulator;
class ASimulatedGuardianManager;
class UGuardianSettings;

UCLASS(Blueprintable)
class GOTA_API AGM_SelfPlay : public AGM_Ingame
{
	GENERATED_BODY()
	AGM_SelfPlay();

	virtual void CreateGuardians() override;

	UPROPERTY()
	UGuardianSettings* GuardianSettings;
	
	TWeakObjectPtr<AGS_SelfPlay> SelfPlayGameState;

	virtual void LoadGame() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGuardian> GuardianClass;

	virtual void BeginPlay() override;
};
