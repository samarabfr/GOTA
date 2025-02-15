// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_SelfPlay.h"

#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "GOTA/GOTARL/GuardianSimulator.h"

AGM_SelfPlay::AGM_SelfPlay()
{
}

void AGM_SelfPlay::CreateGuardians()
{
	ATile* TribeStartingTile = GOTAGameState->GetTileMap()->GetNativesStart().Get();
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
		Guardian->S_Init(GuardianSettings.Get());
		AAIController* GuardianAI = GetWorld()->SpawnActor<AAIController>(
			GuardianAIClass, SpawnLocation, FRotator::ZeroRotator);
		GuardianAI->Possess(Guardian);
	}
}

void AGM_SelfPlay::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
}
