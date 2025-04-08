// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsCommunicator.h"

#include "RL_RunnerManager.generated.h"

class IRL_RunnerAgent;
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
class GOTA_API ARL_RunnerManager : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	ARL_RunnerManager();

	virtual void Tick(float DeltaSeconds) override;

	// ----------------------- Learning Agents plugin -----------------------
private:
	bool bRunTraining = false;
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

	FLearningAgentsCommunicator Communicator;
	FLearningAgentsTrainerProcess TrainerProcess;

	UPROPERTY()
	ULearningAgentsPPOTrainer* PPOTrainer;

public:
	void S_RegisterAgent(UObject* Agent);
	bool IsRegistered(UObject* Agent);
	void S_Init(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
	            ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic, bool RunTraining);
};
