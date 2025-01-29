// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "SettlementPopulation.h"
#include "SettlementSettings.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// --------------------------- Replication Setup ---------------------------

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Settings, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Resources, Params);
}

// --------------------------- LifeCycle ---------------------------

ASettlement::ASettlement()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	SetNetUpdateFrequency(1.0f);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5;

	Population = CreateDefaultSubobject<USettlementPopulation>(TEXT("Population"));
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->IncrementReplicationCount();
}

void ASettlement::S_Init(ATile* SpawnTile,
                         USettlementSettings* InSettlementSettings,
                         UPopulationSettings* InPopulationSettings)
{
	PopulationSettings = InPopulationSettings;
	Settings = InSettlementSettings;

	S_AddResources(Settings->GetStartingResources());

	const TArray<UBuildingSettings*>& StartingBuildings = Settings->GetStartingBuildings();

	SpawnTile->S_TryBuild(StartingBuildings[0], this);
	SpawnTile->GetBuilding()->FinishConstruction();
	for (int32 i = 1; i < StartingBuildings.Num(); ++i)
	{
		if (BorderingUnclaimedTiles.Num() <= 0) break;
		ATile* Tile = BorderingUnclaimedTiles[FMath::RandRange(0, BorderingUnclaimedTiles.Num() - 1)];
		Tile->S_TryBuild(StartingBuildings[i], this);
		Tile->GetBuilding()->FinishConstruction();
	}
	for (ATile* Tile : ClaimedTiles)
	{
		Tile->GetBuilding()->GetPopulation()->S_ChangeSize(100);
	}
}

void ASettlement::EnableTick()
{
	SetActorTickEnabled(true);
}

void ASettlement::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Resources.Food -= GetPopulation()->GetSize() * Settings->GetPopEatingPerSecond() * DeltaSeconds;

	if (Resources.Food < 0)
		PopulationSettings->IsStarving = true;
	else
		PopulationSettings->IsStarving = false;

	if (HasAuthority())
	{
		const float Threshold = Settings->GetStarvingThreshold();
		if (Resources.Food < Threshold)
		{
			int32 Count = Resources.Food / Threshold;
			Resources.Food += Count * Threshold * -1;
			for (int32 i = 0; i < Count; ++i)
			{
				GetPopulation()->StarveRandomPop();
			}
		}
	}
}

// --------------------------- Utility ---------------------------

// --------------------------- Population ---------------------------

// --------------------------- Claims ---------------------------

void ASettlement::RefreshBorderingUnclaimedTiles()
{
	BorderingUnclaimedTiles.Empty();
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		for (ATile* Neighbor : ClaimedTile->Neighbors)
		{
			if (Neighbor && !Neighbor->GetClaimant() && !BorderingUnclaimedTiles.Contains(Neighbor))
			{
				BorderingUnclaimedTiles.Add(Neighbor);
			}
		}
	}
}

bool ASettlement::IsBorderingUnclaimedTile(const ATile* Tile) const
{
	return BorderingUnclaimedTiles.Contains(Tile);
}

// --------------------------- Building ---------------------------

void ASettlement::S_RegisterTile(ATile* Tile)
{
	ClaimedTiles.Add(Tile);
	RefreshBorderingUnclaimedTiles();
}

void ASettlement::RegisterPopulation(UPopulation* InPopulation)
{
	GetPopulation()->RegisterPop(InPopulation);
}

void ASettlement::S_UnregisterTile(ATile* Tile)
{
	ClaimedTiles.Remove(Tile);
	RefreshBorderingUnclaimedTiles();
}

void ASettlement::UnregisterPopulation(UPopulation* InPopulation)
{
	GetPopulation()->UnregisterPop(InPopulation);
}

// --------------------------- Resources ---------------------------


void ASettlement::S_AddResources(const FGameResources Amount)
{
	Resources += Amount;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, Resources, this)
	ForceNetUpdate();
}

void ASettlement::S_RemoveResources(const FGameResources Amount)
{
	Resources -= Amount;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, Resources, this)
	ForceNetUpdate();
}

void ASettlement::RegisterBuildingForResourcePrediction(UBuilding* Building)
{
	const EProductionType ProductionType = Building->GetProductionType();
	if (ProductionType == EProductionType::Food
		|| ProductionType == EProductionType::Wood
		|| ProductionType == EProductionType::Stone)
	{
		PredictedProduction.AddProduction(Building->GetPredictedProduction(), ProductionType);
		Building->OnPredictedProductionChanged.AddDynamic(this, &ASettlement::UpdatePredictedProduction);
	}
	const EConsumptionType ConsumptionType = Building->GetConsumptionType();
	if (ConsumptionType == EConsumptionType::Food
		|| ConsumptionType == EConsumptionType::Wood
		|| ConsumptionType == EConsumptionType::Stone)
	{
		PredictedConsumption.AddConsumption(Building->GetPredictedConsumption(), ConsumptionType);
		Building->OnPredictedConsumptionChanged.AddDynamic(this, &ASettlement::UpdatePredictedConsumption);
	}
	PredictedConsumption.AddConsumption(
		Building->GetPopulation()->GetSize() * Settings->GetPopEatingPerSecond(), EConsumptionType::Food);
	Building->GetPopulation()->OnSizeChanged.AddDynamic(this, &ASettlement::UpdatePredictionFromPopulation);
}

void ASettlement::UnregisterBuildingForResourcePrediction(UBuilding* Building)
{
	const EProductionType ProductionType = Building->GetProductionType();
	if (ProductionType == EProductionType::Food
		|| ProductionType == EProductionType::Wood
		|| ProductionType == EProductionType::Stone)
	{
		PredictedProduction.RemoveProduction(Building->GetPredictedProduction(), ProductionType);
		Building->OnPredictedProductionChanged.RemoveDynamic(this, &ASettlement::UpdatePredictedProduction);
	}
	const EConsumptionType ConsumptionType = Building->GetConsumptionType();
	if (ConsumptionType == EConsumptionType::Food
		|| ConsumptionType == EConsumptionType::Wood
		|| ConsumptionType == EConsumptionType::Stone)
	{
		PredictedConsumption.RemoveConsumption(Building->GetPredictedConsumption(), ConsumptionType);
		Building->OnPredictedConsumptionChanged.RemoveDynamic(this, &ASettlement::UpdatePredictedConsumption);
	}
	PredictedConsumption.RemoveConsumption(
		Building->GetPopulation()->GetSize() * Settings->GetPopEatingPerSecond(), EConsumptionType::Food);
	Building->GetPopulation()->OnSizeChanged.RemoveDynamic(this, &ASettlement::UpdatePredictionFromPopulation);
}

void ASettlement::UpdatePredictedProduction(const float Change, const EProductionType Type)
{
	PredictedProduction.AddProduction(Change, Type);
}

void ASettlement::UpdatePredictedConsumption(const float Change, const EConsumptionType Type)
{
	PredictedConsumption.AddConsumption(Change, Type);
}

void ASettlement::UpdatePredictionFromPopulation(int16 Change)
{
	PredictedConsumption.AddConsumption(Change * Settings->GetPopEatingPerSecond(), EConsumptionType::Food);
}
