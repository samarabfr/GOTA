// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_SelfPlay.h"

#include "GameFramework/GameUserSettings.h"
#include "GOTA/AI/GuardianAIs/GuardianAIController.h"
#include "GOTA/AI/SettlementAIs/SettlementAIController.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Guardian/Guardian.h"
#include "GOTA/Guardian/GuardianSettings.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tilemap/TileMap.h"
#include "PhysicsEngine/PhysicsSettings.h"

#if WITH_EDITOR
#include "Editor/EditorPerformanceSettings.h"
#endif

// ------------------------------------ Lifecycle ------------------------------------

AGM_SelfPlay::AGM_SelfPlay()
{
}

// ------------------------------------ Game ------------------------------------

void AGM_SelfPlay::BeginPlay()
{
	Super::BeginPlay();
	SetGameSettings();
	LoadGame();
}

void AGM_SelfPlay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// logging
	TickCountSinceLastLog++;
	RealTimeLastTick = FPlatformTime::Seconds();
	if (RegularLogDataCooldown <= 0.0f)
	{
		RegularLogDataCooldown = RegularLogDataInterval;
		LogData();
	}
	else
	{
		RegularLogDataCooldown -= DeltaSeconds;
	}
	// Reset when soft locked
	if (SoftLockTimeLeft <= 0.0f)
	{
		SoftLockTimeLeft = SoftLockTime;
		EndGame(EGameEnding::SoftLocked, "Game softlocked");
	}
	else
	{
		SoftLockTimeLeft -= DeltaSeconds;
	}
}

void AGM_SelfPlay::LoadGame()
{
	Super::LoadGame();
	SetupNewRoundLog();
	// reset timers
	SoftLockTimeLeft = SoftLockTime;
}

void AGM_SelfPlay::EndGame(EGameEnding Ending, const FString& EndingMessage)
{
	Super::EndGame(Ending, EndingMessage);
	LogData();
	SaveRoundDataToJson();
	GOTAGameState->DeleteEverything();
	LoadGame();
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
		if (AGuardianAIController* GuardianAI = GOTAGameState->S_GetGuardianAIController(i))
		{
			GuardianAI->Possess(Guardian);
			GuardianAI->S_Init(bRunGuardianAITraining);
		}
		else if (GuardianAIClass)
		{
			GuardianAI = GetWorld()->SpawnActor<AGuardianAIController>(GuardianAIClass);
			GuardianAI->Possess(Guardian);
			GuardianAI->S_Init(bRunGuardianAITraining);
			GOTAGameState->S_SetGuardianAIController(i, GuardianAI);
		}
	}
}

void AGM_SelfPlay::CreateSettlements()
{
	ASettlement* Colony = GetWorld()->SpawnActor<ASettlement>(ColonyClass);
	Colony->S_Init(GOTAGameState->GetTileMap()->GetColonistsStart().Get());
	GOTAGameState->SetColony(Colony);
	if (ASettlementAIController* ColonyAIController = GOTAGameState->S_GetColonyAIController())
	{
		ColonyAIController->Possess(Colony);
		ColonyAIController->S_Init(bRunColonyAITraining);
	}
	else if (ColonyAIControllerClass)
	{
		ColonyAIController = GetWorld()->SpawnActor<ASettlementAIController>(ColonyAIControllerClass);
		ColonyAIController->Possess(Colony);
		ColonyAIController->S_Init(bRunColonyAITraining);
		GOTAGameState->S_SetColonyAIController(ColonyAIController);
	}

	ASettlement* Tribe = GetWorld()->SpawnActor<ASettlement>(TribeClass);
	Tribe->S_Init(GOTAGameState->GetTileMap()->GetNativesStart().Get());
	GOTAGameState->SetTribe(Tribe);
}

// ------------------------------------ soft lock ------------------------------------

// ------------------------------------ Logging ------------------------------------

void AGM_SelfPlay::SetGameSettings()
{
	FApp::SetUseFixedTimeStep(true);
	FApp::SetFixedDeltaTime(FixedDeltaSeconds);
	if (UPhysicsSettings* PhysicsSettings = UPhysicsSettings::Get())
	{
		PhysicsSettings->MaxPhysicsDeltaTime = FixedDeltaSeconds;
	}
	if (IConsoleVariable* MaxFPSCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS")))
	{
		MaxFPSCVar->Set(0);
	}
	if (UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings())
	{
		GameSettings->SetVSyncEnabled(false);
		GameSettings->ApplySettings(false);
	}
	if (UGameViewportClient* ViewportClient = GetWorld() ? GetWorld()->GetGameViewport() : nullptr)
	{
		ViewportClient->ViewModeIndex = EViewModeIndex::VMI_Unlit;
	}

#if WITH_EDITOR
	if (UEditorPerformanceSettings* EditorPerformanceSettings = GetMutableDefault<UEditorPerformanceSettings>())
	{
		EditorPerformanceSettings->bThrottleCPUWhenNotForeground = false;
		EditorPerformanceSettings->bEnableVSync = false;
		EditorPerformanceSettings->PostEditChange();
	}
#endif
}

void AGM_SelfPlay::SaveRoundDataToJson()
{
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("Data/RoundData.log");

	// Create new round JSON object
	TSharedPtr<FJsonObject> NewRound = MakeShareable(new FJsonObject());
	
	NewRound->SetNumberField(TEXT("RoundNumber"), RoundNumber);
	NewRound->SetStringField(TEXT("GameEnding"), UEnum::GetValueAsString(GOTAGameState->GetGameEnding()));
	NewRound->SetArrayField(TEXT("Logs"), RoundLogs);

	// Convert JSON object to string
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(NewRound.ToSharedRef(), Writer);

	// Append new line to log file
	FFileHelper::SaveStringToFile(OutputString + TEXT("\n"), *FilePath, FFileHelper::EEncodingOptions::AutoDetect,
	                              &IFileManager::Get(), FILEWRITE_Append);
}

void AGM_SelfPlay::SetupNewRoundLog()
{
	++RoundNumber;
	GameTimeLastLog = GetWorld()->GetTimeSeconds();
	RealTimeLastLog = FPlatformTime::Seconds();
	RegularLogDataCooldown = RegularLogDataInterval;
	RoundLogs.Empty();
}

void AGM_SelfPlay::LogData()
{
	TSharedPtr<FJsonObject> NewLog = MakeShareable(new FJsonObject());
	
	// times
	const float GameTimeCurrent = GetWorld()->GetTimeSeconds();
	const float GameTimeSinceLast = GameTimeCurrent - GameTimeLastLog;
	const double RealTimeCurrent = FPlatformTime::Seconds();
	const double RealTimeSinceLast = RealTimeCurrent - RealTimeLastLog;
	// logging
	NewLog->SetNumberField(TEXT("GameTime"), GameTimeSinceLast);
	NewLog->SetNumberField(TEXT("RealTime"), RealTimeSinceLast);
	NewLog->SetNumberField(TEXT("TickCount"), TickCountSinceLastLog);
	// reset timers
	GameTimeLastLog = GameTimeCurrent;
	RealTimeLastLog = RealTimeCurrent;
	TickCountSinceLastLog = 0;

	// Settlements
	NewLog->SetObjectField(TEXT("Colony"), GOTAGameState->GetColony()->Log());
	NewLog->SetObjectField(TEXT("Tribe"), GOTAGameState->GetTribe()->Log());

	// AIs
	NewLog->SetObjectField(TEXT("ColonyAI"), GOTAGameState->S_GetColonyAIController()->Log());
	TArray<TSharedPtr<FJsonValue>> GuardianAIControllerLogs = TArray<TSharedPtr<FJsonValue>>();
	for (AGuardianAIController* GuardianAIController : GOTAGameState->S_GetGuardianAIControllers())
	{
		if (GuardianAIController)
		{
			GuardianAIControllerLogs.Add(MakeShareable(new FJsonValueObject(GuardianAIController->Log())));
		}
	}
	NewLog->SetArrayField(TEXT("GuardianAIs"), GuardianAIControllerLogs);

	RoundLogs.Add(MakeShareable(new FJsonValueObject(NewLog)));
}
