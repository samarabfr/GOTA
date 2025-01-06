// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianManager.h"

#include "LearningAgentsCommunicator.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "SimulatedGuardian.h"
#include "SimulatedGuardianInteractor.h"
#include "SimulatedGuardianTrainingEnv.h"
#include "Kismet/GameplayStatics.h"

ASimulatedGuardianManager::ASimulatedGuardianManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickInterval(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ASimulatedGuardianManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ASimulatedGuardianManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bRunInference)
	{
		Policy->RunInference(0.0f);
	}
	else
	{
		FLearningAgentsPPOTrainingSettings TrainingSettings = FLearningAgentsPPOTrainingSettings();
		FLearningAgentsTrainingGameSettings TrainingGameSettings = FLearningAgentsTrainingGameSettings();
		PPOTrainer->RunTraining(TrainingSettings, TrainingGameSettings);
	}
}

void ASimulatedGuardianManager::Init()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimulatedGuardian::StaticClass(), GuardianActors);
	// Interactor
	Interactor = ULearningAgentsInteractor::MakeInteractor(
		ManagerComponent, USimulatedGuardianInteractor::StaticClass(), FName("SimulatedGuardianInteractor"));
	const FSoftObjectPath PathEncoder(TEXT("/Game/GOTARL/DA_SimulatedGuardianEncoder.DA_SimulatedGuardianEncoder"));
	const FSoftObjectPath PathPolicy(TEXT("/Game/GOTARL/DA_SimulatedGuardianPolicy.DA_SimulatedGuardianPolicy"));
	const FSoftObjectPath PathDecoder(TEXT("/Game/GOTARL/DA_SimulatedGuardianDecoder.DA_SimulatedGuardianDecoder"));
	const FSoftObjectPath PathCritic(TEXT("/Game/GOTARL/DA_SimulatedGuardianCritic.DA_SimulatedGuardianCritic"));
	NN_Encoder = Cast<ULearningAgentsNeuralNetwork>(PathEncoder.TryLoad());
	NN_Policy = Cast<ULearningAgentsNeuralNetwork>(PathPolicy.TryLoad());
	NN_Decoder = Cast<ULearningAgentsNeuralNetwork>(PathDecoder.TryLoad());
	NN_Critic = Cast<ULearningAgentsNeuralNetwork>(PathEncoder.TryLoad());
	// Policy
	FLearningAgentsPolicySettings PolicySettings = FLearningAgentsPolicySettings();
	PolicySeed = 1234;
	Policy = ULearningAgentsPolicy::MakePolicy(ManagerComponent, Interactor,
	                                           ULearningAgentsPolicy::StaticClass(),
	                                           FName("SimulatedGuardianPolicy"),
	                                           NN_Encoder,
	                                           NN_Policy,
	                                           NN_Decoder,
	                                           !bRunInference,
	                                           !bRunInference,
	                                           !bRunInference,
	                                           PolicySettings,
	                                           PolicySeed);
	// Critic
	FLearningAgentsCriticSettings CriticSettings = FLearningAgentsCriticSettings();
	CriticSeed = 1234;
	Critic = ULearningAgentsCritic::MakeCritic(ManagerComponent, Interactor, Policy,
	                                           ULearningAgentsCritic::StaticClass(),
	                                           FName("SimulatedGuardianCritic"),
	                                           NN_Critic,
	                                           !bRunInference,
	                                           CriticSettings,
	                                           CriticSeed);
	// Training Environment
	TrainingEnv = ULearningAgentsTrainingEnvironment::MakeTrainingEnvironment(
		ManagerComponent,
		USimulatedGuardianTrainingEnv::StaticClass(),
		FName("SimulatedGuardianTrainingEnvironment"));
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
	// Run Inference Reset
	if (bRunInference)
	{
		// TODO: Reset methods
	}
}

void ASimulatedGuardianManager::RegisterAgent(UObject* Agent)
{
	if (!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}
