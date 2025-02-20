// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsCommunicator.h"
#include "LearningAgentsPPOTrainer.h"

#include "RL_BuildingSelectorManager.generated.h"

struct FMilestone;
enum class EGameEnding : uint8;
class ULearningAgentsPPOTrainer;
class ULearningAgentsTrainingEnvironment;
class ULearningAgentsCritic;
class ULearningAgentsInteractor;
class ULearningAgentsPolicy;
class ULearningAgentsNeuralNetwork;
class ULearningAgentsManager;

// Exists for every AI type.
// Manages all agents of this AI type.

UCLASS(Blueprintable)
class GOTA_API ARL_BuildingSelectorManager : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	ARL_BuildingSelectorManager();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void DoLastTrainingRound(const EGameEnding Ending, const FString& EndMessage);

	// ----------------------- Learning Agents plugin -----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	bool bRunInference = false;
	UPROPERTY(EditDefaultsOnly)
	bool bResetNNsWhenStartingTraining = false;

	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsManager* ManagerComponent;

	UPROPERTY()
	ULearningAgentsInteractor* Interactor;

	UPROPERTY()
	ULearningAgentsPolicy* Policy;
	UPROPERTY(EditDefaultsOnly)
	int32 PolicySeed = 1234;

	UPROPERTY()
	ULearningAgentsCritic* Critic;
	UPROPERTY(EditDefaultsOnly)
	int32 CriticSeed = 1234;

	UPROPERTY()
	ULearningAgentsTrainingEnvironment* TrainingEnv;
	UPROPERTY(EditDefaultsOnly)
	float VictoryReward = 1000;
	UPROPERTY(EditDefaultsOnly)
	float LooseReward = -1000;
	UPROPERTY(EditDefaultsOnly)
	TArray<FMilestone> IncomeRewardMilestones;
	UPROPERTY(EditDefaultsOnly)
	TArray<FMilestone> ResourcesRewardMilestones;

	FLearningAgentsCommunicator Communicator;
	FLearningAgentsTrainerProcess TrainerProcess;

	UPROPERTY()
	ULearningAgentsPPOTrainer* PPOTrainer;
	FLearningAgentsPPOTrainingSettings TrainingSettings;
	FLearningAgentsTrainingGameSettings TrainingGameSettings;

public:
	void S_RegisterAgent(UObject* Agent);
	void S_Init(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
	            ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic);
};
