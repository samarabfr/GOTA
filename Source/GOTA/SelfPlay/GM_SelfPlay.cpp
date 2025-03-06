// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_SelfPlay.h"

#include "AIController.h"
#include "GOTA/CoreSystems/Entity/Builder.h"
#include "GOTA/CoreSystems/Entity/Forager.h"
#include "GOTA/CoreSystems/Entity/Woodcutter.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingArmy.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingCivilian.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDefense.h"
#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/Faction/Settlement/SettlementPopulation.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
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
	const float RealDeltaSeconds = FPlatformTime::Seconds() - LastRealTime;
	if (RealDeltaSeconds > MaxRealTime)
	{
		MaxRealTime = RealDeltaSeconds;
	}
	if (RealDeltaSeconds < MinRealTime)
	{
		MinRealTime = RealDeltaSeconds;
	}
	LastRealTime = RealDeltaSeconds;
	// running log data
	// TODO: show how often won and lost at the end
	if (RegularLogDataCooldown <= 0.0f)
	{
		RegularLogDataCooldown = RegularLogDataInterval;
		LogTimeData();
		LogSettlementData(GOTAGameState->GetColony(), "Colonists");
		LogSettlementData(GOTAGameState->GetTribe(), "Natives");
		GameTimeLastLog = GetWorld()->GetTimeSeconds();
		RealTimeLastLog = FPlatformTime::Seconds();
	}
	else
	{
		RegularLogDataCooldown -= DeltaSeconds;
	}
}

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
	       ),
	       CountBuildings, CountCivilianBuildings, CountArmyBuildings, CountDefenseBuildings)
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
	float GameTimeSinceLast = GetWorld()->GetTimeSeconds() - GameTimeLastLog;
	float RealTimeSinceLast = FPlatformTime::Seconds() - RealTimeLastLog;
	UE_LOG(LogTemp, Warning, TEXT("--------------------------Time Data since last--------------------------"))
	UE_LOG(LogTemp, Warning, TEXT("GameTime average delta: %f, Max: %f, Min: %f"),
	       GameTimeSinceLast / TickCount, MaxDeltaSeconds, MinDeltaSeconds)
	UE_LOG(LogTemp, Warning, TEXT("Realtime average delta: %f, Max: %f, Min: %f"),
		RealTimeSinceLast / TickCount, MaxRealTime, MinRealTime)
	UE_LOG(LogTemp, Warning, TEXT("GameTime total: %f, Realtime total: %f, GameSpeedFactor: %f"),
		   GameTimeSinceLast, RealTimeSinceLast, GameTimeSinceLast / RealTimeSinceLast)
	UE_LOG(LogTemp, Warning, TEXT("--------------------------Time Data since start--------------------------"))
	float GameTimeSinceStart = GetWorld()->GetTimeSeconds() - GameTimeStart;
	float RealTimeSinceStart = FPlatformTime::Seconds() - RealTimeStart;
	UE_LOG(LogTemp, Warning, TEXT("GameTime average delta: %f, Max: %f, Min: %f"),
		   GameTimeSinceStart / TickCount, MaxDeltaSeconds, MinDeltaSeconds)
	UE_LOG(LogTemp, Warning, TEXT("Realtime average delta: %f, Max: %f, Min: %f"),
		RealTimeSinceStart / TickCount, MaxRealTime, MinRealTime)
	UE_LOG(LogTemp, Warning, TEXT("GameTime total: %f, Realtime total: %f, GameSpeedFactor: %f"),
		   GameTimeSinceStart, RealTimeSinceStart, GameTimeSinceStart / RealTimeSinceStart)
}

void AGM_SelfPlay::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
	UGameplayStatics::SetGlobalTimeDilation(this,
	                                        FixedDeltaSeconds / LearningAgentsFixedDeltaSeconds);
	GameTimeStart = GetWorld()->GetTimeSeconds();
	RealTimeStart = FPlatformTime::Seconds();
	LastRealTime = RealTimeStart;
}

void AGM_SelfPlay::EndGame(EGameEnding Ending, const FString& EndingMessage)
{
	Super::EndGame(Ending, EndingMessage);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *EndingMessage)
	LogTimeData();
	LogSettlementData(GOTAGameState->GetColony(), "Colonists");
	LogSettlementData(GOTAGameState->GetTribe(), "Natives");
	UE_LOG(LogTemp, Warning, TEXT("--------------------------Training Data--------------------------"))
	if (Ending == EGameEnding::ColonistsWon)
		++CountColonistsWon;
	else
		++CountNativesWon;
	UE_LOG(LogTemp, Warning, TEXT("Count Colonists won: %d, Count Natives won: %d"),
	       CountColonistsWon, CountNativesWon)
	S_RestartSelfPlay();
}

void AGM_SelfPlay::S_RestartSelfPlay()
{
	// Delete Everything
	GOTAGameState->DeleteEverything();
	// Load from the beginning
	LoadGame();
}
