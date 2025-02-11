// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsCommunicator.h"

#include "SimulatedGuardianManager.generated.h"

class ULearningAgentsPPOTrainer;
class ULearningAgentsTrainingEnvironment;
class ULearningAgentsCritic;
class ULearningAgentsInteractor;
class ULearningAgentsPolicy;
class ULearningAgentsNeuralNetwork;
class ULearningAgentsManager;

UCLASS(Blueprintable)
class GOTA_API ASimulatedGuardianManager : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	ASimulatedGuardianManager();

	virtual void Tick(float DeltaSeconds) override;

	// ----------------------- Learning Agents plugin -----------------------
private:
	bool bRunInference = true;
	bool bResetNNsWhenStartingTraining = false;

	UPROPERTY()
	ULearningAgentsManager* ManagerComponent;
	UPROPERTY()
	TArray<AActor*> GuardianSimulatorActors;

	UPROPERTY()
	ULearningAgentsNeuralNetwork* NN_Critic;
	UPROPERTY()
	ULearningAgentsNeuralNetwork* NN_Encoder;
	UPROPERTY()
	ULearningAgentsNeuralNetwork* NN_Policy;
	UPROPERTY()
	ULearningAgentsNeuralNetwork* NN_Decoder;

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

	void Init();

public:
	void RegisterAgent(UObject* Agent);

	void SaveModel(FFilePath& FilePath, FString ModelName);
	void LoadModel(FFilePath& FilePath, FString ModelName);
};
