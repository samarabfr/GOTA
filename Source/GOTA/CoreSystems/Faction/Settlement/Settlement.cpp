// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "SettlementPopulation.h"
#include "GOTA/CoreSystems/Entity/Builder.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingCivilian.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
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

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Resources, Params);
	DOREPLIFETIME_WITH_PARAMS(ASettlement, PopEatingPerSecond, Params)
	DOREPLIFETIME_WITH_PARAMS(ASettlement, StarvingThreshold, Params)
	DOREPLIFETIME_WITH_PARAMS(ASettlement, GrowthPerOwnPop, Params)
	DOREPLIFETIME_WITH_PARAMS(ASettlement, GrowthPerNeighborPop, Params)
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

void ASettlement::Delete()
{
	Destroy();
}

void ASettlement::S_Init(ATile* SpawnTile)
{
	S_AddResources(StartingResources);

	if (SpawnTile->S_TryForceBuild(StartingBuildings[0], this))
	{
		SpawnTile->GetBuilding()->FinishConstruction();
	}
	for (int32 i = 1; i < StartingBuildings.Num(); ++i)
	{
		if (BorderingUnclaimedTiles.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Did not have enough bordering tiles to place starting buildings"))
			break;
		}
		ATile* Tile = BorderingUnclaimedTiles[FMath::RandRange(0, BorderingUnclaimedTiles.Num() - 1)];
		if (Tile && Tile->S_TryBuild(StartingBuildings[i], this))
		{
			Tile->GetBuilding()->FinishConstruction();
		}
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

	Resources.Food -= GetPopulation()->GetSize() * GetPopEatingPerSecond() * DeltaSeconds;

	if (Resources.Food < 0)
		Population->S_SetStarving(true);
	else
		Population->S_SetStarving(false);

	if (HasAuthority())
	{
		const float Threshold = GetStarvingThreshold();
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


void ASettlement::OnRep_PopEatingPerSecond(const float OldValue)
{
	OnPopEatingPerSecondChanged.Broadcast(PopEatingPerSecond - OldValue);
}

void ASettlement::S_SetPopEatingPerSecond(float NewValue)
{
	const float Change = NewValue - PopEatingPerSecond;
	PopEatingPerSecond = NewValue;
	OnPopEatingPerSecondChanged.Broadcast(Change);
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, PopEatingPerSecond, this)
}

void ASettlement::S_SetStarvingThreshold(float NewValue)
{
	StarvingThreshold = NewValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, StarvingThreshold, this)
}

// --------------------------- Claims ---------------------------

void ASettlement::RefreshBorderingUnclaimedTiles()
{
	BorderingUnclaimedTiles.Empty();
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		for (ATile* Neighbor : ClaimedTile->Neighbors)
		{
			if (Neighbor &&
				!Neighbor->IsClaimed() &&
				!ClaimedTiles.Contains(Neighbor) &&
				!BorderingUnclaimedTiles.Contains(Neighbor))
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


void ASettlement::S_AddResources(const FConstructionResources Amount)
{
	Resources += Amount;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, Resources, this)
	ForceNetUpdate();
}

void ASettlement::S_RemoveResources(const FConstructionResources Amount)
{
	Resources -= Amount;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, Resources, this)
	ForceNetUpdate();
}

int32 ASettlement::GetCountOfBuilders()
{
	int32 Count = 0;
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		if (ClaimedTile &&
			ClaimedTile->GetBuilding() &&
			ClaimedTile->GetBuilding()->GetSettings()->bCivilianEnabled)
		{
			UBuildingCivilian* Building = Cast<UBuildingCivilian>(ClaimedTile->GetBuilding());
			if (Building &&
				Building->GetCivilian() &&
				Cast<ABuilder>(Building->GetCivilian()) != nullptr)
			{
				++Count;
			}
		}		
	}
	return Count;
}

int32 ASettlement::GetCountOfConstructionSites()
{
	int32 Count = 0;
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		if (ClaimedTile &&
			ClaimedTile->GetBuilding() &&
			ClaimedTile->GetBuilding()->GetIsUnderConstruction())
		{
			++Count;
		}		
	}
	return Count;
}

void ASettlement::RegisterBuildingForResourcePrediction(UBuilding* Building)
{
	const EProductionType ProductionType = Building->GetProductionType();
	if (ProductionType == EProductionType::Food
		|| ProductionType == EProductionType::Wood
		|| ProductionType == EProductionType::Stone)
	{
		PredictedProduction.Add(Building->GetPredictedProduction(), ProductionType);
		Building->OnPredictedProductionChanged.AddDynamic(this, &ASettlement::UpdatePredictedProduction);
	}
	const EConsumptionType ConsumptionType = Building->GetConsumptionType();
	if (ConsumptionType == EConsumptionType::Food
		|| ConsumptionType == EConsumptionType::Wood
		|| ConsumptionType == EConsumptionType::Stone)
	{
		PredictedConsumption.Add(Building->GetPredictedConsumption(), ConsumptionType);
		Building->OnPredictedConsumptionChanged.AddDynamic(this, &ASettlement::UpdatePredictedConsumption);
	}
	PredictedConsumption.Add(
		Building->GetPopulation()->GetSize() * GetPopEatingPerSecond(), EConsumptionType::Food);
	Building->GetPopulation()->OnSizeChanged.AddDynamic(this, &ASettlement::UpdatePredictionFromPopulation);
}

void ASettlement::UnregisterBuildingForResourcePrediction(UBuilding* Building)
{
	const EProductionType ProductionType = Building->GetProductionType();
	if (ProductionType == EProductionType::Food
		|| ProductionType == EProductionType::Wood
		|| ProductionType == EProductionType::Stone)
	{
		PredictedProduction.Remove(Building->GetPredictedProduction(), ProductionType);
		Building->OnPredictedProductionChanged.RemoveDynamic(this, &ASettlement::UpdatePredictedProduction);
	}
	const EConsumptionType ConsumptionType = Building->GetConsumptionType();
	if (ConsumptionType == EConsumptionType::Food
		|| ConsumptionType == EConsumptionType::Wood
		|| ConsumptionType == EConsumptionType::Stone)
	{
		PredictedConsumption.Remove(Building->GetPredictedConsumption(), ConsumptionType);
		Building->OnPredictedConsumptionChanged.RemoveDynamic(this, &ASettlement::UpdatePredictedConsumption);
	}
	PredictedConsumption.Remove(
		Building->GetPopulation()->GetSize() * GetPopEatingPerSecond(), EConsumptionType::Food);
	Building->GetPopulation()->OnSizeChanged.RemoveDynamic(this, &ASettlement::UpdatePredictionFromPopulation);
}

void ASettlement::UpdatePredictedProduction(const float Change, const EProductionType Type)
{
	PredictedProduction.Add(Change, Type);
}

void ASettlement::UpdatePredictedConsumption(const float Change, const EConsumptionType Type)
{
	PredictedConsumption.Add(Change, Type);
}

void ASettlement::UpdatePredictionFromPopulation(int16 Change)
{
	PredictedConsumption.Add(Change * GetPopEatingPerSecond(), EConsumptionType::Food);
}
