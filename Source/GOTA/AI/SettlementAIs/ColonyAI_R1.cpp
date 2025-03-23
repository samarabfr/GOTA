// Fill out your copyright notice in the Description page of Project Settings.


#include "ColonyAI_R1.h"

#include "LearningAgentsNeuralNetwork.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/ReinforcementLearning/BuildingSelector/RL_BuildingSelectorManager.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/BuildingSettings.h"


void AColonyAI_R1::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GetPossessedSettlement())
		return;
	if (GetPossessedSettlement()->CanAddConstructionSite())
	{
		BuildingSelector->SelectBuilding();
	}
	if (bRunTraining && bSaveSnapshotsAtIntervals && BuildingSelector && !BuildingSelector->IsPaused())
	{
		const double CurrentTime = FPlatformTime::Seconds();
		if (CurrentTime - RealTimeLastSnapshotSave >= SaveSnapshotsIntervalTime)
		{
			RealTimeLastSnapshotSave = CurrentTime;
			SaveModel(FDateTime::Now().ToString());
		}
	}
}

AColonyAI_R1::AColonyAI_R1()
{
}

void AColonyAI_R1::S_Init(bool RunTraining)
{
	Super::S_Init(RunTraining);
	if (!GameState)
		return;
	BuildingSelector = GameState->S_GetRLManager<ARL_BuildingSelectorManager>(ManagerClass);
	if (!BuildingSelector)
	{
		BuildingSelector = GetWorld()->SpawnActor<ARL_BuildingSelectorManager>(ManagerClass,
			FVector::Zero(),
			FRotator::ZeroRotator);
		BuildingSelector->S_Init(NN_Encoder, NN_Policy, NN_Decoder, NN_Critic, bRunTraining);
		AddTickPrerequisiteActor(BuildingSelector); // make the manager tick before this
		GameState->S_AddManager(ManagerClass, BuildingSelector);
	}
	if (!BuildingSelector->IsRegistered(this))
	{
		BuildingSelector->S_RegisterAgent(this);
	}
	if (BuildingSelector->IsPaused())
	{
		BuildingSelector->Unpause();
	}
}

void AColonyAI_R1::Possess(ASettlement* Settlement)
{
	Super::Possess(Settlement);
	if (HasAuthority())
	{
		GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
		MilestonesReached.SetNumZeroed(6);
		// Buildingscounter
		BuildingsCounter.Empty();
		for (UBuildingSettings* Building : PossibleBuildings)
		{
			BuildingsCounter.Add(Building->Name, 0);
		}
	}
}

void AColonyAI_R1::RandomlyPlaceBuilding(UBuildingSettings* Building)
{
	if (!GetPossessedSettlement() || GetPossessedSettlement()->BorderingUnclaimedTiles.Num() <= 0) return;
	const int32 RandomIndex = FMath::RandRange(0, GetPossessedSettlement()->BorderingUnclaimedTiles.Num() - 1);
	ATile* Tile = GetPossessedSettlement()->BorderingUnclaimedTiles[RandomIndex];
	if (!Tile || !Tile->CanBuild(Building, GetPossessedSettlement()))
	{
		return;
	}
	if (Tile->S_TryBuild(Building, GetPossessedSettlement()))
	{
		BuildingsCounter[Building->Name]++;
	}
}

ASettlement* AColonyAI_R1::GetSettlement()
{
	return GetPossessedSettlement();
}

TArray<UBuildingSettings*> AColonyAI_R1::GetAvailableBuildings()
{
	return PossibleBuildings;
}

void AColonyAI_R1::HandleBuildingSelected(UBuildingSettings* Building)
{
	RandomlyPlaceBuilding(Building);
}

EAffiliation AColonyAI_R1::GetAffiliation()
{
	return EAffiliation::Enemy;
}

TArray<int32> AColonyAI_R1::GetMilestonesReached()
{
	return MilestonesReached;
}

void AColonyAI_R1::IncrementMilestone(int32 MilestoneIndex)
{
	if (MilestoneIndex >= MilestonesReached.Num()) return;
	MilestonesReached[MilestoneIndex] += 1;
}

void AColonyAI_R1::SaveModel(const FString& ModelName)
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
	UE_LOG(LogTemp, Warning, TEXT("Saving Model to: %s"), *ModelPath.FilePath)
}

void AColonyAI_R1::LoadModel(const FString& ModelName)
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
	UE_LOG(LogTemp, Warning, TEXT("Loading Model from: %s"), *ModelPath.FilePath)
}

FString AColonyAI_R1::GetAgentName()
{
	return SnapshotAgentName;
}

void AColonyAI_R1::LogBuildings()
{
	UE_LOG(LogTemp, Warning, TEXT("--------- Building Counts ---------"))
	for (auto Counter : BuildingsCounter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %d"), *Counter.Key.ToString(), Counter.Value)
	}
}

void AColonyAI_R1::LogBuildingSelector()
{
	UE_LOG(LogTemp, Warning, TEXT("--------- Building Selector ---------"))
	UE_LOG(LogTemp, Warning, TEXT("Step num: %d"), BuildingSelector->GetStepNum(this))
}

void AColonyAI_R1::Log()
{
	UE_LOG(LogTemp, Warning,
	       TEXT("-------------------------- Colony AI - R1 --------------------------"))
	LogBuildings();
	LogBuildingSelector();
}
