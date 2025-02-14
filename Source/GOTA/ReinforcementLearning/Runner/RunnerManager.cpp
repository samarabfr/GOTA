// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerManager.h"

#include "LearningAgentsCommunicator.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "RunnerInteractor.h"
#include "RunnerTrainingEnv.h"
#include "Kismet/GameplayStatics.h"

ARunnerManager::ARunnerManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickInterval(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ARunnerManager::Tick(float DeltaSeconds)
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

void ARunnerManager::Init()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGuardianSimulator::StaticClass(), GuardianSimulatorActors);
	// make the manager tick before the simulators
	for (AActor* GuardianSimulatorActor : GuardianSimulatorActors)
	{
		if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GuardianSimulatorActor))
		{
			GuardianSimulator->AddTickPrerequisiteActor(this);
		}
	}
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
	NN_Critic = Cast<ULearningAgentsNeuralNetwork>(PathCritic.TryLoad());
	// Policy
	FLearningAgentsPolicySettings PolicySettings = FLearningAgentsPolicySettings();
	PolicySeed = 1234;
	Policy = ULearningAgentsPolicy::MakePolicy(ManagerComponent, Interactor,
	                                           ULearningAgentsPolicy::StaticClass(),
	                                           FName("SimulatedGuardianPolicy"),
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
	                                           FName("SimulatedGuardianCritic"),
	                                           NN_Critic,
	                                           !bRunInference && bResetNNsWhenStartingTraining,
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
		for (AActor* GuardianSimulatorActor : GuardianSimulatorActors)
		{
			if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GuardianSimulatorActor))
			{
				GuardianSimulator->ResetToRandomTile();
			}
		}
	}
}

void ARunnerManager::RegisterAgent(UObject* Agent)
{
	if (!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
	Init();
}

void ARunnerManager::SaveModel(FFilePath& FilePath, FString ModelName)
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

void ARunnerManager::LoadModel(FFilePath& FilePath, FString ModelName)
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
