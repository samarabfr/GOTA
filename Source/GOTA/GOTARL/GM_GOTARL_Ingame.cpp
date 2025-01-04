// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_GOTARL_Ingame.h"

#include "GOTA/CoreSystems/Guardian/GuardianSettings.h"
#include "SimulatedGuardian.h"
#include "SimulatedGuardianManager.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"

AGM_GOTARL_Ingame::AGM_GOTARL_Ingame()
{
	ConstructorHelpers::FObjectFinder<UGuardianSettings> DataAssetFinder(
		TEXT("/Game/Guardians/Flamey/DA_Flamey"));
	if (DataAssetFinder.Succeeded())
	{
		GuardianSettings = DataAssetFinder.Object;
	}
}

void AGM_GOTARL_Ingame::CreateGuardians()
{
	ATile* TribeStartingTile = GOTAGameState->GetTileMap()->GetNativesStart().Get();
	LearningManager = GetWorld()->SpawnActor<ASimulatedGuardianManager>();
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
		ASimulatedGuardian* Guardian = GetWorld()->SpawnActor<ASimulatedGuardian>(SpawnLocation, FRotator::ZeroRotator);
		Guardian->Init(GuardianSettings);
		LearningManager->RegisterAgent(Guardian);
	}
}

void AGM_GOTARL_Ingame::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
}
