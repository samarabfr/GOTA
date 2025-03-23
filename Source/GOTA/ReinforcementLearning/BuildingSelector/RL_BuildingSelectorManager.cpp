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
#include "GOTA/GameplayFramework/GS_Ingame.h"

ARL_BuildingSelectorManager::ARL_BuildingSelectorManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickInterval(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ARL_BuildingSelectorManager::DoLastTrainingRound(const EGameEnding Ending, const FString& EndMessage)
{
	// TODO: make own training loop. With RunTraining the model does an action
	PPOTrainer->RunTraining(TrainingSettings, TrainingGameSettings);
	Pause();
}

void ARL_BuildingSelectorManager::S_Init(ULearningAgentsNeuralNetwork* NN_Encoder,
                                         ULearningAgentsNeuralNetwork* NN_Policy,
                                         ULearningAgentsNeuralNetwork* NN_Decoder,
                                         ULearningAgentsNeuralNetwork* NN_Critic,
                                         bool RunTraining)
{
	bRunTraining = RunTraining;
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	if (GameState && bRunTraining)
	{
		GameState->OnGameEnding.AddDynamic(this, &ARL_BuildingSelectorManager::DoLastTrainingRound);
	}
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
	                                           bRunTraining && bResetNNsWhenStartingTraining,
	                                           bRunTraining && bResetNNsWhenStartingTraining,
	                                           bRunTraining && bResetNNsWhenStartingTraining,
	                                           PolicySettings,
	                                           PolicySeed);
	// Critic
	FLearningAgentsCriticSettings CriticSettings = FLearningAgentsCriticSettings();
	CriticSeed = 1234;
	Critic = ULearningAgentsCritic::MakeCritic(ManagerComponent, Interactor, Policy,
	                                           ULearningAgentsCritic::StaticClass(),
	                                           FName("BuildingSelectorCritic"),
	                                           NN_Critic,
	                                           bRunTraining && bResetNNsWhenStartingTraining,
	                                           CriticSettings,
	                                           CriticSeed);
	// Training Environment
	TrainingEnvironment = ULearningAgentsTrainingEnvironment::MakeTrainingEnvironment(
		ManagerComponent,
		URL_BuildingSelectorTrainingEnv::StaticClass(),
		FName("BuildingSelectorTrainingEnvironment"));
	if (URL_BuildingSelectorTrainingEnv* BSTrainingEnv = Cast<URL_BuildingSelectorTrainingEnv>(TrainingEnvironment))
	{
		BSTrainingEnv->Init(GameState, VictoryReward, LooseReward, IncomeRewardMilestones, ResourcesRewardMilestones);
	}
	// Shared Memory
	FLearningAgentsTrainerProcessSettings TrainerProcessSettings = FLearningAgentsTrainerProcessSettings();
	TrainerProcessSettings.NonEditorEngineRelativePath = NonEditorEngineRelativePath;
	TrainerProcessSettings.NonEditorIntermediateRelativePath = NonEditorIntermediateRelativePath;
	FLearningAgentsSharedMemoryCommunicatorSettings SharedMemorySettings =
		FLearningAgentsSharedMemoryCommunicatorSettings();
	TrainerProcess = ULearningAgentsCommunicatorLibrary::SpawnSharedMemoryTrainingProcess(
		TrainerProcessSettings, SharedMemorySettings);
	Communicator = ULearningAgentsCommunicatorLibrary::MakeSharedMemoryCommunicator(
		TrainerProcess, SharedMemorySettings);
	// PPO Trainer
	FLearningAgentsPPOTrainerSettings TrainerSettings = FLearningAgentsPPOTrainerSettings();
	PPOTrainer = ULearningAgentsPPOTrainer::MakePPOTrainer(
		ManagerComponent, Interactor, TrainingEnvironment, Policy, Critic, Communicator,
		ULearningAgentsPPOTrainer::StaticClass(), FName("PPOTrainer"), TrainerSettings);
	TrainingSettings = FLearningAgentsPPOTrainingSettings();
	TrainingSettings.bUseTensorboard = bUseTensorboard;
	TrainingGameSettings = FLearningAgentsTrainingGameSettings();
}

bool ARL_BuildingSelectorManager::IsPaused() const
{
	return bPaused;
}

void ARL_BuildingSelectorManager::Pause()
{
	bPaused = true;
}

void ARL_BuildingSelectorManager::Unpause()
{
	bPaused = false;
}

void ARL_BuildingSelectorManager::S_RegisterAgent(UObject* Agent)
{
	if (!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}

bool ARL_BuildingSelectorManager::IsRegistered(UObject* Agent) const
{
	if (!ManagerComponent || !Agent) return false;
	return ManagerComponent->HasAgentObject(Agent);
}

void ARL_BuildingSelectorManager::SelectBuilding()
{
	if (IsPaused()) return;
	if (!bRunTraining)
	{
		Policy->RunInference(0.0f);
	}
	else
	{
		PPOTrainer->RunTraining(TrainingSettings, TrainingGameSettings);
	}
}

int32 ARL_BuildingSelectorManager::GetStepNum(UObject* Agent) const
{
	return PPOTrainer->GetEpisodeStepNum(ManagerComponent->GetAgentId(Agent));
}
