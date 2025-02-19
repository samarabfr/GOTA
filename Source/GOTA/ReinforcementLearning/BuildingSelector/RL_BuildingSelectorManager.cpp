// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorManager.h"

#include "LearningAgentsCommunicator.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "RL_BuildingSelectorInteractor.h"
#include "RL_BuildingSelectorTrainingEnv.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Kismet/GameplayStatics.h"

ARL_BuildingSelectorManager::ARL_BuildingSelectorManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickInterval(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ARL_BuildingSelectorManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bRunInference)
	{
		Policy->RunInference(0.0f);
	}
	else
	{
		FLearningAgentsPPOTrainingSettings TrainingSettings = FLearningAgentsPPOTrainingSettings();
		TrainingSettings.bUseTensorboard = true;
		FLearningAgentsTrainingGameSettings TrainingGameSettings = FLearningAgentsTrainingGameSettings();
		PPOTrainer->RunTraining(TrainingSettings, TrainingGameSettings);
	}
}

void ARL_BuildingSelectorManager::S_Init(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
                                   ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic)
{
	// Interactor
	Interactor = ULearningAgentsInteractor::MakeInteractor(
		ManagerComponent, URL_BuildingSelectorInteractor::StaticClass(), FName("BuildingSelectorInteractor"));
	// Policy
	FLearningAgentsPolicySettings PolicySettings = FLearningAgentsPolicySettings();
	PolicySeed = 1234;
	Policy = ULearningAgentsPolicy::MakePolicy(ManagerComponent, Interactor,
	                                           ULearningAgentsPolicy::StaticClass(),
	                                           FName("BuildingSelectorPolicy"),
	                                           NN_Encoder,
	                                           NN_Policy,
	                                           NN_Decoder,
	                                           !bRunInference && bResetNNsWhenStartingTraining,
	                                           !bRunInference && bResetNNsWhenStartingTraining,
	                                           !bRunInference && bResetNNsWhenStartingTraining,
	                                           PolicySettings,
	                                           PolicySeed);
	// Critic
	FLearningAgentsCriticSettings CriticSettings = FLearningAgentsCriticSettings();
	CriticSeed = 1234;
	Critic = ULearningAgentsCritic::MakeCritic(ManagerComponent, Interactor, Policy,
	                                           ULearningAgentsCritic::StaticClass(),
	                                           FName("BuildingSelectorCritic"),
	                                           NN_Critic,
	                                           !bRunInference && bResetNNsWhenStartingTraining,
	                                           CriticSettings,
	                                           CriticSeed);
	// Training Environment
	TrainingEnv = ULearningAgentsTrainingEnvironment::MakeTrainingEnvironment(
		ManagerComponent,
		URL_BuildingSelectorTrainingEnv::StaticClass(),
		FName("BuildingSelectorTrainingEnvironment"));
	// Shared Memory
	FLearningAgentsTrainerProcessSettings TrainerProcessSettings = FLearningAgentsTrainerProcessSettings();
	FLearningAgentsSharedMemoryCommunicatorSettings SharedMemorySettings =
		FLearningAgentsSharedMemoryCommunicatorSettings();
	TrainerProcess = ULearningAgentsCommunicatorLibrary::SpawnSharedMemoryTrainingProcess(
		TrainerProcessSettings, SharedMemorySettings);
	Communicator = ULearningAgentsCommunicatorLibrary::MakeSharedMemoryCommunicator(
		TrainerProcess, SharedMemorySettings);
	// PPO Trainer
	FLearningAgentsPPOTrainerSettings TrainerSettings = FLearningAgentsPPOTrainerSettings();
	PPOTrainer = ULearningAgentsPPOTrainer::MakePPOTrainer(
		ManagerComponent, Interactor, TrainingEnv, Policy, Critic, Communicator,
		ULearningAgentsPPOTrainer::StaticClass(), FName("PPOTrainer"), TrainerSettings);
}

void ARL_BuildingSelectorManager::S_RegisterAgent(UObject* Agent)
{
	if (!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}
