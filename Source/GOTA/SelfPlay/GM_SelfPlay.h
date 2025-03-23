// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/Settlement/Settlement.h"
#include "GOTA/GameplayFramework/GM_Ingame.h"

#include "GM_SelfPlay.generated.h"


class AGuardianAIController;
class AGS_SelfPlay;
class AGuardianSimulator;
class ASimulatedGuardianManager;
class UGuardianSettings;

UCLASS(Blueprintable)
class GOTA_API AGM_SelfPlay : public AGM_Ingame
{
	GENERATED_BODY()

	// ------------------------------------ Lifecycle ------------------------------------
protected:
	AGM_SelfPlay();

	// ------------------------------------ Game ------------------------------------
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void LoadGame() override;
	virtual void EndGame(EGameEnding Ending, const FString& EndingMessage) override;
	void S_RestartSelfPlay();

	virtual void CreateGuardians() override;
	virtual void CreateSettlements() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<UGuardianSettings*> GuardianSettings;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGuardianAIController> GuardianAIClass;
	UPROPERTY(EditDefaultsOnly)
	bool bRunGuardianAITraining = true;
	UPROPERTY(EditDefaultsOnly)
	bool bRunColonyAITraining = true;

	;

	// ------------------------------------ Game Settings ------------------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	float FixedDeltaSeconds = 0.1f;

	void SetGameSettings();

	// ------------------------------------ soft lock ------------------------------------

private:
	UPROPERTY(EditDefaultsOnly)
	float SoftLockTime = 18000.0f; // GameTime in seconds
	float SoftLockTimeLeft = 0.0f;

	// ------------------------------------ Logging ------------------------------------

private:
	UPROPERTY(EditDefaultsOnly)
	float RegularLogDataInterval = 900.0f; // GameTime in seconds

	float RegularLogDataCooldown = 0.0f;
	void LogSettlementData(ASettlement* Settlement, FString SettlementName);
	int32 CountColonistsWon = 0;
	int32 CountNativesWon = 0;
	int32 CountSoftLocked = 0;
	// Time data
	void LogTimeData();
	int32 TickCount = 0;
	// GameTime
	float GameTimeLastLog = 0.0f;
	float GameTimeStart = 0.0f;
	float MaxDeltaSeconds = 0.0f;
	float MinDeltaSeconds = FLT_MAX;
	// RealTime
	double RealTimeStart = 0.0f;
	double RealTimeLastLog = 0.0f;
	double LastRealTime = 0.0f;
	double MaxRealTime = 0.0f;
	double MinRealTime = DBL_MAX;
	// AI
	void LogAIControllers();
};
