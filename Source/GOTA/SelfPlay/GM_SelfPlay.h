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
	
	virtual void StartGame() override;
	virtual void EndGame(EGameEnding Ending, const FString& EndingMessage) override;

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

	// save data only in c++ until the end of the round and then write it to the JSON file

	double RealTimeLastTick = 0.0f;
	FString LogName = "UnnamedLog";
	
	// -------------------- Round --------------------
	void SaveRoundDataToJson();
	void SetupNewRoundLog();
	int32 RoundNumber = 0;
	TArray<TSharedPtr<FJsonValue>> RoundLogs = TArray<TSharedPtr<FJsonValue>>();
	
	// -------- Since Last Log --------
	void LogData();
	int32 TickCountSinceLastLog = 0;
	float GameTimeLastLog = 0.0f;
	double RealTimeLastLog = 0.0f;
	double SavingLastLogTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float RegularLogDataInterval = 900.0f; // GameTime in seconds
	float RegularLogDataCooldown = 0.0f;
};
