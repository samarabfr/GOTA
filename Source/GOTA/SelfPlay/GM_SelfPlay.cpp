// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_SelfPlay.h"

#include "GS_SelfPlay.h"
#include "GOTA/CoreSystems/Guardian/GuardianSettings.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "GOTA/GOTARL/GuardianSimulator.h"
#include "GOTA/GOTARL/SimulatedGuardianManager.h"

AGM_SelfPlay::AGM_SelfPlay()
{
	ConstructorHelpers::FObjectFinder<UGuardianSettings> DataAssetFinder(
		TEXT("/Game/Guardians/Flamey/DA_Flamey"));
	if (DataAssetFinder.Succeeded())
	{
		GuardianSettings = DataAssetFinder.Object;
	}
}

void AGM_SelfPlay::CreateGuardians()
{
	ATile* TribeStartingTile = GOTAGameState->GetTileMap()->GetNativesStart().Get();
	SelfPlayGameState->SetLearningManager(GetWorld()->SpawnActor<ASimulatedGuardianManager>());
	for (int32 i = 0; i < 1; ++i)
	{
		FVector SpawnLocation = FVector(0, 0, 1000);
		if (TribeStartingTile->Neighbors[i])
		{
			SpawnLocation = TribeStartingTile->Neighbors[i]->GetActorLocation() + FVector(0, 0, 100);
		}
		else
		{
			SpawnLocation = TribeStartingTile->GetActorLocation() + FVector(0, 0, 100);
		}
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(
			GuardianClass, SpawnLocation, FRotator::ZeroRotator);
		Guardian->S_Init(GuardianSettings);
		AGuardianSimulator* GuardianSimulator = GetWorld()->SpawnActor<AGuardianSimulator>(
			AGuardianSimulator::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
		GuardianSimulator->Possess(Guardian);
		SelfPlayGameState->GetLearningManager()->RegisterAgent(GuardianSimulator);
	}
}

void AGM_SelfPlay::LoadGame()
{
	Super::LoadGame();
	SelfPlayGameState = GetGameState<AGS_SelfPlay>();
}

void AGM_SelfPlay::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
}
