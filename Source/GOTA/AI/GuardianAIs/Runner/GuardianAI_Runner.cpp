// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianAI_Runner.h"

#include "LearningAgentsNeuralNetwork.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tilemap/TileMap.h"
#include "GOTA/ReinforcementLearning/Runner/RL_RunnerManager.h"
#include "GOTA/Tile/Tile.h"

AGuardianAI_Runner::AGuardianAI_Runner()
{
}


void AGuardianAI_Runner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bWantsToMove)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector());
	}
	if (!TargetTile.IsValid() &&
		GameState.IsValid() &&
		GameState->GetTileMap())
	{
		TargetTile = GameState->GetTileMap()->GetVolcanoTile();
	}
}

void AGuardianAI_Runner::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (HasAuthority())
	{
		GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
		ARL_RunnerManager* Manager = GameState->S_GetRLManager<ARL_RunnerManager>(ManagerClass);
		if (!Manager)
		{
			Manager = GetWorld()->SpawnActor<ARL_RunnerManager>(ManagerClass);
			Manager->S_Init(NN_Encoder, NN_Policy, NN_Decoder, NN_Critic, GameState->S_GetRunGuardianAITraining());
			AddTickPrerequisiteActor(Manager); // make the manager tick before this
			GameState->S_AddManager(ManagerClass, Manager);
		}
		if (!Manager->IsRegistered(this))
		{
			Manager->S_RegisterAgent(this);
		}
	}
}

FTransform AGuardianAI_Runner::S_GetAgentTransform() const
{
	return GetPawn()->GetActorTransform();
}

void AGuardianAI_Runner::S_SetIsMoving(bool NewIsMoving)
{
	bWantsToMove = NewIsMoving;
}

ATile* AGuardianAI_Runner::S_GetTargetTile() const
{
	return TargetTile.Get();
}

void AGuardianAI_Runner::S_ResetToRandomTile()
{
	if (!GameState.IsValid() || !GameState->GetTileMap()) return;
	const ATile* RandomTile = GameState->GetTileMap()->GetRandomTile();
	GetPawn()->TeleportTo(RandomTile->GetActorTransform().GetLocation() + ResetOffset,
						  RandomTile->GetActorTransform().GetRotation().Rotator());
}

void AGuardianAI_Runner::S_Steer(float SteeringAngle)
{
	if (SteeringAngle == 0.f) return;
	GetPawn()->AddActorLocalRotation(FRotator(0.f, SteeringAngle, 0.f));
}

void AGuardianAI_Runner::SaveModel(const FString& ModelName)
{
	FFilePath ModelPath;
	ModelPath.FilePath = FPaths::ProjectContentDir() / SnapshotsFolderFilePath.FilePath / ModelName;
	FFilePath FullSnapshotPath;
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Critic";
	NN_Critic->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Encoder";
	NN_Encoder->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Policy";
	NN_Policy->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Decoder";
	NN_Decoder->SaveNetworkToSnapshot(FullSnapshotPath);
}

void AGuardianAI_Runner::LoadModel(const FString& ModelName)
{
	FFilePath ModelPath;
	ModelPath.FilePath = FPaths::ProjectContentDir() / SnapshotsFolderFilePath.FilePath / ModelName;
	FFilePath FullSnapshotPath;
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Critic";
	NN_Critic->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Encoder";
	NN_Encoder->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Policy";
	NN_Policy->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Decoder";
	NN_Decoder->LoadNetworkFromSnapshot(FullSnapshotPath);
}

FString AGuardianAI_Runner::GetAgentName()
{
	return SnapshotAgentName;
}
