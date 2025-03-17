// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_SelfPlay.h"

#include "AIController.h"
#include "GOTA/AI/GuardianAIs/GuardianAIController.h"
#include "GOTA/AI/SettlementAIs/SettlementAIController.h"
#include "GOTA/Entity/Builder.h"
#include "GOTA/Entity/Forager.h"
#include "GOTA/Entity/Woodcutter.h"
#include "GOTA/Tile/Building/BuildingArmy.h"
#include "GOTA/Tile/Building/BuildingCivilian.h"
#include "GOTA/Tile/Building/BuildingDefense.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Settlement/SettlementPopulation.h"
#include "GOTA/Guardian/Guardian.h"
#include "GOTA/Guardian/GuardianSettings.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tilemap/TileMap.h"
#include "Kismet/GameplayStatics.h"
// ------------------------------------ Lifecycle ------------------------------------

AGM_SelfPlay::AGM_SelfPlay()
{
}

// ------------------------------------ Game ------------------------------------

void AGM_SelfPlay::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
	UGameplayStatics::SetGlobalTimeDilation(this,
	                                        FixedDeltaSeconds / LearningAgentsFixedDeltaSeconds);
}

void AGM_SelfPlay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// calculate time data
	TickCount++;
	if (DeltaSeconds > MaxDeltaSeconds)
	{
		MaxDeltaSeconds = DeltaSeconds;
	}
	if (DeltaSeconds < MinDeltaSeconds)
	{
		MinDeltaSeconds = DeltaSeconds;
	}
	const double CurrentRealTime = FPlatformTime::Seconds();
	const double RealDeltaSeconds = CurrentRealTime - LastRealTime;
	if (RealDeltaSeconds > MaxRealTime)
	{
		MaxRealTime = RealDeltaSeconds;
	}
	if (RealDeltaSeconds < MinRealTime)
	{
		MinRealTime = RealDeltaSeconds;
	}
	LastRealTime = CurrentRealTime;
	// running log data
	if (RegularLogDataCooldown <= 0.0f)
	{
		RegularLogDataCooldown = RegularLogDataInterval;
		LogTimeData();
		LogSettlementData(GOTAGameState->GetColony(), "Colonists");
		LogSettlementData(GOTAGameState->GetTribe(), "Natives");
		LogAIControllers();
		GameTimeLastLog = GetWorld()->GetTimeSeconds();
		RealTimeLastLog = CurrentRealTime;
	}
	else
	{
		RegularLogDataCooldown -= DeltaSeconds;
	}
	// Reset when soft locked
	if (SoftLockTimeLeft <= 0.0f)
	{
		SoftLockTimeLeft = SoftLockTime;
		++CountSoftLocked;
		UE_LOG(LogTemp, Warning, TEXT("Game soft locked after %f seconds. Soft locked %d in total. Resetting..."),
		       SoftLockTime, CountSoftLocked)
		S_RestartSelfPlay();
	}
	else
	{
		SoftLockTimeLeft -= DeltaSeconds;
	}
}

void AGM_SelfPlay::LoadGame()
{
	Super::LoadGame();
	// reset timers
	SoftLockTimeLeft = SoftLockTime;
	RegularLogDataCooldown = RegularLogDataInterval;
	TickCount = 0;
	// GameTime
	GameTimeStart = GetWorld()->GetTimeSeconds();
	GameTimeLastLog = GameTimeStart;
	MaxDeltaSeconds = 0.0f;
	MinDeltaSeconds = FLT_MAX;
	// RealTime
	RealTimeStart = FPlatformTime::Seconds();
	RealTimeLastLog = RealTimeStart;
	LastRealTime = RealTimeStart;
	MaxRealTime = 0.0f;
	MinRealTime = DBL_MAX;
}

void AGM_SelfPlay::EndGame(EGameEnding Ending, const FString& EndingMessage)
{
	Super::EndGame(Ending, EndingMessage);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *EndingMessage)
	LogTimeData();
	LogSettlementData(GOTAGameState->GetColony(), "Colonists");
	LogSettlementData(GOTAGameState->GetTribe(), "Natives");
	LogAIControllers();
	UE_LOG(LogTemp, Warning, TEXT("--------------------------Training Data--------------------------"))
	if (Ending == EGameEnding::ColonistsWon)
		++CountColonistsWon;
	else
		++CountNativesWon;
	UE_LOG(LogTemp, Warning, TEXT("Count Colonists won: %d, Count Natives won: %d, Count Soft-locked: %d"),
	       CountColonistsWon, CountNativesWon, CountSoftLocked)
	const float TotalGames = CountColonistsWon + CountNativesWon + CountSoftLocked;
	UE_LOG(LogTemp, Warning, TEXT("Colonists winrate: %f, Natives winrate: %f, Soft-locked rate: %f"),
	       static_cast<float>(CountColonistsWon) / TotalGames,
	       static_cast<float>(CountNativesWon) / TotalGames,
	       static_cast<float>(CountSoftLocked) / TotalGames)
	S_RestartSelfPlay();
}

void AGM_SelfPlay::S_RestartSelfPlay()
{
	// Delete Everything
	GOTAGameState->DeleteEverything();
	// Load from the beginning
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
		if (AGuardianAIController* GuardianAI = GOTAGameState->GetGuardianAIController(i))
		{
			GuardianAI->Possess(Guardian);
		}
		else if (GuardianAIClass)
		{
			GuardianAI = GetWorld()->SpawnActor<AGuardianAIController>(GuardianAIClass);
			GuardianAI->Possess(Guardian);
			GOTAGameState->SetGuardianAIController(i, GuardianAI);
		}
	}
}

// ------------------------------------ soft lock ------------------------------------

// ------------------------------------ Logging ------------------------------------

void AGM_SelfPlay::LogSettlementData(ASettlement* Settlement, FString SettlementName)
{
	if (!Settlement)
		return;
	UE_LOG(LogTemp, Warning, TEXT("--------------------------%s Data--------------------------"), *SettlementName)
	// Buildings
	int32 CountBuildings = Settlement->GetAllBuildings().Num();
	int32 CountCivilianBuildings = 0;
	int32 CountArmyBuildings = 0;
	int32 CountDefenseBuildings = 0;
	for (UBuilding* Building : Settlement->GetAllBuildings())
	{
		if (Cast<UBuildingCivilian>(Building))
			++CountCivilianBuildings;
		else if (Cast<UBuildingDefense>(Building))
			++CountDefenseBuildings;
		else if (Cast<UBuildingArmy>(Building))
			++CountArmyBuildings;
	}
	UE_LOG(LogTemp, Warning,
	       TEXT("Count Buildings: %d, Count CivilianBuildings: %d, Count ArmyBuildings: %d, Count DefenseBuildings: %d"
	       ), CountBuildings, CountCivilianBuildings, CountArmyBuildings, CountDefenseBuildings)
	// Civilians
	int32 CountCivilians = Settlement->GetAllCivilians().Num();
	int32 CountWoodcutter = 0;
	int32 CountForager = 0;
	int32 CountBuilder = 0;
	for (ACivilian* Civilian : Settlement->GetAllCivilians())
	{
		if (Cast<AWoodcutter>(Civilian))
			++CountWoodcutter;
		else if (Cast<AForager>(Civilian))
			++CountForager;
		else if (Cast<ABuilder>(Civilian))
			++CountBuilder;
	}
	UE_LOG(LogTemp, Warning, TEXT("Count Civilians: %d, Count Woodcutter: %d, Count Forager: %d, Count Builder: %d"),
	       CountCivilians, CountWoodcutter, CountForager, CountBuilder)
	// Armies
	UE_LOG(LogTemp, Warning, TEXT("Count Armies: %d"), Settlement->GetAllArmies().Num())
	// Pop
	UE_LOG(LogTemp, Warning, TEXT("Count Pop: %d"), Settlement->GetPopulation()->GetSize())
	// Resources
	UE_LOG(LogTemp, Warning, TEXT("Food: %f, Wood: %f, Stone: %f"),
	       Settlement->GetResources().Food, Settlement->GetResources().Wood, Settlement->GetResources().Stone)
	UE_LOG(LogTemp, Warning, TEXT("Food income: %f, Wood income: %f, Stone income: %f"),
	       Settlement->GetEffectiveProduction().Food, Settlement->GetEffectiveProduction().Wood,
	       Settlement->GetEffectiveProduction().Stone)
}

void AGM_SelfPlay::LogTimeData()
{
	const float GameTimeCurrent = GetWorld()->GetTimeSeconds();
	const float GameTimeSinceLast = GameTimeCurrent - GameTimeLastLog;
	const double RealTimeCurrent = FPlatformTime::Seconds();
	const double RealTimeSinceLast = RealTimeCurrent - RealTimeLastLog;
	UE_LOG(LogTemp, Warning, TEXT("--------------------------Time Data since last--------------------------"))
	UE_LOG(LogTemp, Warning, TEXT("GameTime average delta: %f, Max: %f, Min: %f"),
	       GameTimeSinceLast / static_cast<float>(TickCount), MaxDeltaSeconds, MinDeltaSeconds)
	UE_LOG(LogTemp, Warning, TEXT("Realtime average delta: %f, Max: %f, Min: %f"),
	       RealTimeSinceLast / static_cast<double>(TickCount), MaxRealTime, MinRealTime)
	UE_LOG(LogTemp, Warning, TEXT("GameTime total: %f, Realtime total: %f, GameSpeedFactor: %f"),
	       GameTimeSinceLast, RealTimeSinceLast, GameTimeSinceLast / RealTimeSinceLast)
	UE_LOG(LogTemp, Warning, TEXT("--------------------------Time Data since start--------------------------"))
	const float GameTimeSinceStart = GameTimeCurrent - GameTimeStart;
	const double RealTimeSinceStart = RealTimeCurrent - RealTimeStart;
	UE_LOG(LogTemp, Warning, TEXT("GameTime average delta: %f, Max: %f, Min: %f"),
	       GameTimeSinceStart / static_cast<float>(TickCount), MaxDeltaSeconds, MinDeltaSeconds)
	UE_LOG(LogTemp, Warning, TEXT("Realtime average delta: %f, Max: %f, Min: %f"),
	       RealTimeSinceStart / static_cast<double>(TickCount), MaxRealTime, MinRealTime)
	UE_LOG(LogTemp, Warning, TEXT("GameTime total: %f, Realtime total: %f, GameSpeedFactor: %f"),
	       GameTimeSinceStart, RealTimeSinceStart, GameTimeSinceStart / RealTimeSinceStart)
}

void AGM_SelfPlay::LogAIControllers()
{
	if (GOTAGameState->GetColonyAIController())
		GOTAGameState->GetColonyAIController()->Log();
	for (AGuardianAIController* GuardianAIController : GOTAGameState->GetGuardianAIControllers())
	{
		if (GuardianAIController)
			GuardianAIController->Log();
	}
}
