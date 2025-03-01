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
		GOTAGameState->SetGuardian(1, Guardian);
		Guardian->S_Init(GuardianSettings[i], PossibleBuildingsForPlayers);
		if (AAIController* GuardianAI = GOTAGameState->GetGuardianAIController(i))
		{
			GuardianAI->Possess(Guardian);
		}
		else if (GuardianAIClass)
		{
			GuardianAI = GetWorld()->SpawnActor<AAIController>(
				GuardianAIClass, SpawnLocation, FRotator::ZeroRotator);
			GuardianAI->Possess(Guardian);
			GOTAGameState->SetGuardianAIController(i, GuardianAI);
		}
	}
}

void AGM_SelfPlay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UE_LOG(LogTemp, Warning, TEXT("GameTime tick: %f"), DeltaSeconds)
	TickCount++;
	if (DeltaSeconds > MaxDeltaSeconds)
	{
		MaxDeltaSeconds = DeltaSeconds;
	}
	if (DeltaSeconds < MinDeltaSeconds)
	{
		MinDeltaSeconds = DeltaSeconds;
	}
	const float RealDeltaSeconds = FPlatformTime::Seconds()- LastRealTime;
	if (RealDeltaSeconds > MaxRealTime)
	{
		MaxRealTime = RealDeltaSeconds;
	}
	if (RealDeltaSeconds < MinRealTime)
	{
		MinRealTime = RealDeltaSeconds;
	}
	LastRealTime = RealDeltaSeconds;
}

void AGM_SelfPlay::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
	// set max time dilation
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
	GameTimeStart = GetWorld()->GetTimeSeconds();
	RealTimeStart = FPlatformTime::Seconds();
	LastRealTime = RealTimeStart;
}

void AGM_SelfPlay::EndGame(EGameEnding Ending, const FString& EndingMessage)
{
	Super::EndGame(Ending, EndingMessage);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *EndingMessage)
	UE_LOG(LogTemp, Warning, TEXT("GameTime average delta: %f, Max: %f, Min: %f"),
	       (GetWorld()->GetTimeSeconds()- GameTimeStart ) / TickCount, MaxDeltaSeconds, MinDeltaSeconds)
	UE_LOG(LogTemp, Warning, TEXT("Realtime average delta: %f, Max: %f, Min: %f"),
	       (FPlatformTime::Seconds() - RealTimeStart) / TickCount, MaxRealTime, MinRealTime)
	RestartSelfPlay();
}

void AGM_SelfPlay::RestartSelfPlay()
{
	// Delete Everything
	GOTAGameState->DeleteEverything();
	// Load from the beginning
	LoadGame();
}
