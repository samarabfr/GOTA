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
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ASimulatedGuardianManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ASimulatedGuardianManager::Init()
{
	TArray<AActor*> GuardianActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimulatedGuardian::StaticClass(), GuardianActors);
	// Interactor
	ULearningAgentsInteractor* Interactor = ULearningAgentsInteractor::MakeInteractor(
		ManagerComponent, USimulatedGuardianInteractor::StaticClass(), FName("SimulatedGuardianInteractor"));
	// Policy
	bool bRunInference = false;
	const FSoftObjectPath PathEncoder(TEXT("/Game/GOTARL/DA_SimulatedGuardianEncoder.DA_SimulatedGuardianEncoder"));
	const FSoftObjectPath PathPolicy(TEXT("/Game/GOTARL/DA_SimulatedGuardianPolicy.DA_SimulatedGuardianPolicy"));
	const FSoftObjectPath PathDecoder(TEXT("/Game/GOTARL/DA_SimulatedGuardianDecoder.DA_SimulatedGuardianDecoder"));
	ULearningAgentsNeuralNetwork* NN_Encoder = Cast<ULearningAgentsNeuralNetwork>(PathEncoder.TryLoad());
	ULearningAgentsNeuralNetwork* NN_Policy = Cast<ULearningAgentsNeuralNetwork>(PathPolicy.TryLoad());
	ULearningAgentsNeuralNetwork* NN_Decoder = Cast<ULearningAgentsNeuralNetwork>(PathDecoder.TryLoad());
	FLearningAgentsPolicySettings PolicySettings = FLearningAgentsPolicySettings();
	int32 PolicySeed = 1234;
	ULearningAgentsPolicy* Policy = ULearningAgentsPolicy::MakePolicy(ManagerComponent, Interactor,
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
	const FSoftObjectPath PathCritic(TEXT("/Game/GOTARL/DA_SimulatedGuardianCritic.DA_SimulatedGuardianCritic"));
	ULearningAgentsNeuralNetwork* NN_Critic = Cast<ULearningAgentsNeuralNetwork>(PathEncoder.TryLoad());
	FLearningAgentsCriticSettings CriticSettings = FLearningAgentsCriticSettings();
	int32 CriticSeed = 1234;
	ULearningAgentsCritic* Critic = ULearningAgentsCritic::MakeCritic(ManagerComponent, Interactor, Policy,
	                                                                  ULearningAgentsCritic::StaticClass(),
	                                                                  FName("SimulatedGuardianCritic"),
	                                                                  NN_Critic,
	                                                                  true,
	                                                                  CriticSettings,
	                                                                  CriticSeed);
	// Training Environment
	ULearningAgentsTrainingEnvironment* TraingEnv = ULearningAgentsTrainingEnvironment::MakeTrainingEnvironment(
		ManagerComponent,
		USimulatedGuardianTrainingEnv::StaticClass(),
		FName("SimulatedGuardianTrainingEnvironment"));
	// Shared Memory
	FLearningAgentsTrainerProcessSettings TrainerProcessSettings = FLearningAgentsTrainerProcessSettings();
	FLearningAgentsSharedMemoryCommunicatorSettings SharedMemorySettings =
		FLearningAgentsSharedMemoryCommunicatorSettings();
	FLearningAgentsTrainerProcess TrainerProcess = ULearningAgentsCommunicatorLibrary::SpawnSharedMemoryTrainingProcess(
		TrainerProcessSettings, SharedMemorySettings);
	FLearningAgentsCommunicator Communicator = ULearningAgentsCommunicatorLibrary::MakeSharedMemoryCommunicator(
		TrainerProcess, SharedMemorySettings);
	// PPO Trainer
	FLearningAgentsPPOTrainerSettings TrainerSettings = FLearningAgentsPPOTrainerSettings();
	ULearningAgentsPPOTrainer* PPOTrainer = ULearningAgentsPPOTrainer::MakePPOTrainer(
		ManagerComponent, Interactor, TraingEnv, Policy, Critic, Communicator,
		ULearningAgentsPPOTrainer::StaticClass(), FName("PPOTrainer"), TrainerSettings);
}

void ASimulatedGuardianManager::RegisterAgent(UObject* Agent)
{
	if (!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}
