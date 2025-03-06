// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/GameplayFramework/GM_Ingame.h"

#include "GM_SelfPlay.generated.h"


class AAIController;
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

	UPROPERTY(EditDefaultsOnly)
	TArray<UGuardianSettings*> GuardianSettings;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAIController> GuardianAIClass;

	UPROPERTY(EditDefaultsOnly)
	float FixedDeltaSeconds = 0.1f;
	float LearningAgentsFixedDeltaSeconds = 1.0f/60.0f;

	virtual void Tick(float DeltaSeconds) override;

	int32 TickCount = 0;
	float GameTimeStart = 0.0f;
	float MaxDeltaSeconds = 0.0f;
	float MinDeltaSeconds = FLT_MAX;
	float RealTimeStart = 0.0f;
	float LastRealTime = 0.0f;
	float MaxRealTime = 0.0f;
	float MinRealTime = FLT_MAX;

	virtual void BeginPlay() override;
	virtual void EndGame(EGameEnding Ending, const FString& EndingMessage) override;

	void S_RestartSelfPlay();
};
