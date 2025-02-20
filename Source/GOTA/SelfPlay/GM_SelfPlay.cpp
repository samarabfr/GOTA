// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_SelfPlay.h"

#include "AIController.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Guardian/GuardianSettings.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "Kismet/GameplayStatics.h"

AGM_SelfPlay::AGM_SelfPlay()
{
}

void AGM_SelfPlay::CreateGuardians()
{
	ATile* TribeStartingTile = GOTAGameState->GetTileMap()->GetNativesStart().Get();
	for (int32 i = 0; i < GuardianSettings.Num(); ++i)
	{
		FVector SpawnLocation = FVector(0, 0, 1000);
		int32 TileIndex = i % TribeStartingTile->Neighbors.Num();
		if (TribeStartingTile->Neighbors[TileIndex])
		{
			SpawnLocation = TribeStartingTile->Neighbors[TileIndex]->GetActorLocation() + FVector(0, 0, 100);
		}
		else
		{
			SpawnLocation = TribeStartingTile->GetActorLocation() + FVector(0, 0, 100);
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AGuardian* Guardian = GetWorld()->SpawnActor<AGuardian>(
			GuardianSettings[i]->GuardianBlueprint, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		Guardian->S_Init(GuardianSettings[i], PossibleBuildingsForPlayers);
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

void AGM_SelfPlay::EndGame(EGameEnding Ending, const FString& EndingMessage)
{
	Super::EndGame(Ending, EndingMessage);
	RestartSelfPlay();
}

void AGM_SelfPlay::RestartSelfPlay()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FName CurrentLevelName = *World->GetMapName();
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}
