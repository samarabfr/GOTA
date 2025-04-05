// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/AI/GuardianAIs/GuardianAIController.h"
#include "GOTA/ReinforcementLearning/BuildingSelector/RL_BuildingSelectorAgent.h"
#include "GOTA/ReinforcementLearning/SnapshotSystem/SnapshotAgent.h"

#include "GuardianAI_BuildingSelector.generated.h"

class ASettlement;
class AGuardian;
class ULearningAgentsManager;
class ARL_BuildingSelectorManager;
class AGS_Ingame;
class ULearningAgentsNeuralNetwork;
class ATile;

UCLASS(Blueprintable)
class GOTA_API AGuardianAI_BuildingSelector : public AGuardianAIController, public IRL_BuildingSelectorAgent, public ISnapshotAgent
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------

private:
	virtual void Tick(float DeltaSeconds) override;

protected:
	AGuardianAI_BuildingSelector();

public:
	virtual void S_Init(bool RunTraining = false) override;


	// ----------------------- Utility -----------------------
private:
	UPROPERTY()
	AGS_Ingame* GameState;
	UPROPERTY()
	AGuardian* PossessedGuardian;
	UPROPERTY()
	ASettlement* Settlement;
	virtual void OnPossess(APawn* InPawn) override;

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
	TArray<ATile*> FindTilesWithMostNeighborPop() const;
	TArray<int32> MilestonesReached;

public:
	virtual ASettlement* GetSettlement() override;
	virtual ASettlement* GetEnemySettlement() override;
	virtual TArray<UBuildingSettings*> GetAvailableBuildings() override;
	virtual void HandleBuildingActionSelected(UBuildingSettings* Building) override;
	virtual EAffiliation GetAffiliation() override;
	virtual TArray<int32> GetMilestonesReached() override;
	virtual void IncrementMilestone(int32 MilestoneIndex) override;
	virtual void SaveModel(const FString& ModelName) override;
	virtual void LoadModel(const FString& ModelName) override;
	virtual FString GetAgentName() override;
	virtual bool CanBuild() override;
	
	// --------------------Logging----------------------
private:
	TMap<FName, int32> BuildingsCounter;

public:
	virtual TSharedPtr<FJsonObject> Log() override;
};
