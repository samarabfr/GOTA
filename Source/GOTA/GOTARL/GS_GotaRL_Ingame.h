// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GS_GotaRL_Ingame.generated.h"


class ASimulatedGuardianManager;

UCLASS()
class GOTA_API AGS_GotaRL_Ingame : public AGS_Ingame
{
	GENERATED_BODY()

	// ------------------- Manager -------------------

	TWeakObjectPtr<ASimulatedGuardianManager> LearningManager;

public:
	ASimulatedGuardianManager* GetLearningManager();
	void SetLearningManager(ASimulatedGuardianManager* NewLearningManager);
};
