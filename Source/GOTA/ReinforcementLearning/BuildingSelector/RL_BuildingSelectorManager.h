// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsCommunicator.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsPolicy.h"
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

	UFUNCTION()
	void HandleGameEnding(const EGameEnding Ending, const FString& EndMessage);

	// ----------------------- Learning Agents plugin -----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsManager* ManagerComponent;

	bool bRunTraining = false;
	bool bResetNNsWhenStartingTraining = false;

	UPROPERTY()
	ULearningAgentsInteractor* Interactor;

	UPROPERTY()
	ULearningAgentsPolicy* Policy;
	UPROPERTY(EditDefaultsOnly)
	int32 PolicySeed = 1234;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsPolicySettings PolicySettings;

	UPROPERTY()
	ULearningAgentsCritic* Critic;
	UPROPERTY(EditDefaultsOnly)
	int32 CriticSeed = 1234;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsCriticSettings CriticSettings;

	UPROPERTY()
	ULearningAgentsTrainingEnvironment* TrainingEnvironment;
	UPROPERTY(EditDefaultsOnly)
	float VictoryReward = 1000;
	UPROPERTY(EditDefaultsOnly)
	float LooseReward = -1000;

	FLearningAgentsCommunicator Communicator;
	FLearningAgentsTrainerProcess TrainerProcess;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsTrainerProcessSettings TrainerProcessSettings;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsSharedMemoryCommunicatorSettings SharedMemorySettings;
	
	UPROPERTY()
	ULearningAgentsPPOTrainer* PPOTrainer;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsPPOTrainerSettings TrainerSettings;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsPPOTrainingSettings TrainingSettings;
	UPROPERTY(EditDefaultsOnly)
	FLearningAgentsTrainingGameSettings TrainingGameSettings;

	bool bPaused = false;
	bool bIsFirstStepAfterReset = false;

public:
	void S_RegisterAgent(UObject* Agent);
	void S_Init(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
	            ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic, bool RunTraining);
	bool IsPaused() const;
	void Pause();
	void Unpause();
	bool IsRegistered(UObject* Agent) const;
	void SelectBuildingAction();
	int32 GetStepNum(UObject* Agent) const;
};
