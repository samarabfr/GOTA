// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementAIController.h"
#include "GOTA/ReinforcementLearning/BuildingSelector/RL_BuildingSelectorAgent.h"
#include "GOTA/ReinforcementLearning/SnapshotSystem/SnapshotAgent.h"
#include "ColonyAI_R1.generated.h"

class AGS_Ingame;
class ARL_BuildingSelectorManager;
class ULearningAgentsNeuralNetwork;

UCLASS()
class GOTA_API AColonyAI_R1 : public ASettlementAIController, public IRL_BuildingSelectorAgent, public ISnapshotAgent
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
private:
	virtual void Tick(float DeltaSeconds) override;

protected:
	AColonyAI_R1();

public:
	virtual void Possess(ASettlement* Settlement) override;

	// ----------------------- Utility -----------------------
private:
	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> PossibleBuildings;

	// ----------------------- Reinforcement Learning -----------------------
private:
	UPROPERTY(EditDefaultsOnly, Category="Snapshot")
	FString SnapshotAgentName = "Unnamed";
	UPROPERTY(EditDefaultsOnly, Category="Snapshot")
	bool bSaveSnapshotsAtIntervals = true;
	UPROPERTY(EditDefaultsOnly, Category="Snapshot")
	double SaveSnapshotsIntervalTime = 900.0f;
	UPROPERTY(EditDefaultsOnly, Category="Snapshot")
	FFilePath SnapshotsFolderFilePath;

	UPROPERTY(EditDefaultsOnly, Category="LearningAgents")
	ULearningAgentsNeuralNetwork* NN_Encoder;
	UPROPERTY(EditDefaultsOnly, Category="LearningAgents")
	ULearningAgentsNeuralNetwork* NN_Policy;
	UPROPERTY(EditDefaultsOnly, Category="LearningAgents")
	ULearningAgentsNeuralNetwork* NN_Decoder;
	UPROPERTY(EditDefaultsOnly, Category="LearningAgents")
	ULearningAgentsNeuralNetwork* NN_Critic;
	UPROPERTY(EditDefaultsOnly, Category="LearningAgents")
	TSubclassOf<ARL_BuildingSelectorManager> ManagerClass;

	UPROPERTY()
	ARL_BuildingSelectorManager* BuildingSelector;
	double RealTimeLastSnapshotSave = 0.0f;

	void RandomlyPlaceBuilding(UBuildingSettings* Building);
	TArray<int32> MilestonesReached;

public:
	virtual ASettlement* GetSettlement() override;
	virtual TArray<UBuildingSettings*> GetAvailableBuildings() override;
	virtual void HandleBuildingSelected(UBuildingSettings* Building) override;
	virtual EAffiliation GetAffiliation() override;
	virtual TArray<int32> GetMilestonesReached() override;
	virtual void IncrementMilestone(int32 MilestoneIndex) override;
	virtual void SaveModel(const FString& ModelName) override;
	virtual void LoadModel(const FString& ModelName) override;
	virtual FString GetAgentName() override;

	// -------------------- Logging ----------------------
private:
	TMap<FName, int32> BuildingsCounter;
	void LogBuildings();

public:
	virtual void Log() override;
};
