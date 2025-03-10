// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"
#include "GOTA/ReinforcementLearning/Runner/RL_RunnerAgent.h"
#include "GOTA/ReinforcementLearning/SnapshotSystem/SnapshotAgent.h"

#include "GuardianAI_Runner.generated.h"

class ULearningAgentsManager;
class ARL_RunnerManager;
class AGS_Ingame;
class ULearningAgentsNeuralNetwork;
class ATile;

UCLASS(Blueprintable)
class GOTA_API AGuardianAI_Runner : public AAIController, public IRL_RunnerAgent, public ISnapshotAgent
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

protected:
	AGuardianAI_Runner();

	
	// ----------------------- Utility -----------------------
private:
	TWeakObjectPtr<AGS_Ingame> GameState;

	// ----------------------- Movement control -----------------------
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
	TSubclassOf<ARL_RunnerManager> ManagerClass;
	
	UPROPERTY(EditDefaultsOnly)
	FVector ResetOffset = FVector(0, 0, 50);
	
	bool bWantsToMove = false;

	UPROPERTY(EditInstanceOnly)
	TWeakObjectPtr<ATile> TargetTile;

public:
	virtual FTransform S_GetAgentTransform() const override;
	virtual void S_SetIsMoving(bool NewIsMoving) override;
	virtual ATile* S_GetTargetTile() const override;
	virtual void S_ResetToRandomTile() override;
	virtual void S_Steer(float SteeringAngle) override;
	virtual void SaveModel(const FString& ModelName) override;
	virtual void LoadModel(const FString& ModelName) override;
	virtual FString GetAgentName() override;
};
