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
	bool bRunInference = false;
	bool bResetNNsWhenStartingTraining = false;

	UPROPERTY()
	ULearningAgentsManager* ManagerComponent;
	UPROPERTY()
	TArray<AActor*> GuardianSimulatorActors;

	UPROPERTY()
	ULearningAgentsInteractor* Interactor;

	UPROPERTY()
	ULearningAgentsPolicy* Policy;
	int32 PolicySeed = 1234;

	UPROPERTY()
	ULearningAgentsCritic* Critic;
	int32 CriticSeed = 1234;

	UPROPERTY()
	ULearningAgentsTrainingEnvironment* TrainingEnv;

	FLearningAgentsCommunicator Communicator;
	FLearningAgentsTrainerProcess TrainerProcess;

	UPROPERTY()
	ULearningAgentsPPOTrainer* PPOTrainer;

	void InitObject(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
	                ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic);
	void RegisterAgentOnObject(UObject* Agent);

public:
	static void RegisterAgent(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
	                          ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic,
	                          TScriptInterface<IRL_RunnerAgent> RunnerAgent);


	// TODO: Let the Agents manage the NNs and snapshots themselves
	/*
	void SaveModel(FFilePath& FilePath, FString ModelName);
	void LoadModel(FFilePath& FilePath, FString ModelName);
	*/
};
