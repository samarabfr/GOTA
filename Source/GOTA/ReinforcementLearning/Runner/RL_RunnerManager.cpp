// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_RunnerManager.h"

#include "LearningAgentsCommunicator.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "RL_RunnerAgent.h"
#include "RL_RunnerInteractor.h"
#include "RL_RunnerTrainingEnv.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Kismet/GameplayStatics.h"

ARL_RunnerManager::ARL_RunnerManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickInterval(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ARL_RunnerManager::Tick(float DeltaSeconds)
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

void ARL_RunnerManager::S_Init(ULearningAgentsNeuralNetwork* NN_Encoder, ULearningAgentsNeuralNetwork* NN_Policy,
                                   ULearningAgentsNeuralNetwork* NN_Decoder, ULearningAgentsNeuralNetwork* NN_Critic)
{
	// Interactor
	Interactor = ULearningAgentsInteractor::MakeInteractor(
		ManagerComponent, URL_RunnerInteractor::StaticClass(), FName("RunnerInteractor"));
	// Policy
	FLearningAgentsPolicySettings PolicySettings = FLearningAgentsPolicySettings();
	PolicySeed = 1234;
	Policy = ULearningAgentsPolicy::MakePolicy(ManagerComponent, Interactor,
	                                           ULearningAgentsPolicy::StaticClass(),
	                                           FName("RunnerPolicy"),
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
	                                           FName("RunnerCritic"),
	                                           NN_Critic,
	                                           !bRunInference && bResetNNsWhenStartingTraining,
	                                           CriticSettings,
	                                           CriticSeed);
	// Training Environment
	TrainingEnv = ULearningAgentsTrainingEnvironment::MakeTrainingEnvironment(
		ManagerComponent,
		URL_RunnerTrainingEnv::StaticClass(),
		FName("RunnerTrainingEnvironment"));
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

void ARL_RunnerManager::S_RegisterAgent(UObject* Agent)
{
	if (!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}

/*
void ARL_RunnerManager::SaveModel(FFilePath& FilePath, FString ModelName)
{
	FFilePath FullSnapshotPath;
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Critic";
	NN_Critic->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Encoder";
	NN_Encoder->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Policy";
	NN_Policy->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Decoder";
	NN_Decoder->SaveNetworkToSnapshot(FullSnapshotPath);
}

void ARL_RunnerManager::LoadModel(FFilePath& FilePath, FString ModelName)
{
	FFilePath FullSnapshotPath;
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Critic";
	NN_Critic->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Encoder";
	NN_Encoder->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Policy";
	NN_Policy->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = FilePath.FilePath / ModelName + "Decoder";
	NN_Decoder->LoadNetworkFromSnapshot(FullSnapshotPath);
}
*/
