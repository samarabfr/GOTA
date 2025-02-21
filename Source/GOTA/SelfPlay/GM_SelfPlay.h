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
	FString IslandPath = "/Game/SelfPlay/SelfPlay_Island";

	UPROPERTY(EditDefaultsOnly)
	float TimeDilation = 20.0f;

	virtual void BeginPlay() override;
	virtual void EndGame(EGameEnding Ending, const FString& EndingMessage) override;

	void RestartSelfPlay();
};
