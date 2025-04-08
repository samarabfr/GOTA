// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "SettlementPopulation.h"
#include "GOTA/Entity/Army.h"
#include "GOTA/Entity/Builder.h"
#include "GOTA/Entity/Forager.h"
#include "GOTA/Entity/Woodcutter.h"
#include "GOTA/Tile/Building/Building.h"
#include "GOTA/Tile/Building/BuildingCivilian.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/Utility/ResourceStorage.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/BuildingArmy.h"
#include "GOTA/Tile/Building/BuildingDefense.h"
#include "GOTA/Tile/Building/Population.h"
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
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Consumption, Params)
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Production, Params)
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
	if (HasAuthority())
	{
		Destroy();
	}
}

void ASettlement::S_Init(ATile* SpawnTile)
{
	S_AddResources(StartingResources);

	if (SpawnTile->S_TryForceBuild(StartingBuildings[0], this))
	{
		SpawnTile->GetBuilding()->S_FinishConstruction();
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
			Tile->GetBuilding()->S_FinishConstruction();
		}
	}
	for (ATile* Tile : ClaimedTiles)
	{
		if (Tile &&
			Tile->GetBuilding())
		{
			Tile->GetBuilding()->GetPopulation()->S_ChangeSize(100);
			Tile->GetBuilding()->GetResourceStorage()->S_Fill();
		}
	}
}

void ASettlement::EnableTick()
{
	SetActorTickEnabled(true);
}

void ASettlement::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		S_AddResources(Production - Consumption);
		// starving
		if (Resources.Food < 0)
			Population->S_SetStarving(true);
		else
			Population->S_SetStarving(false);
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
	else
	{
		C_AddResources(Production - Consumption);
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

void ASettlement::S_RefreshBorderingUnclaimedTiles()
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

void ASettlement::S_RegisterTile(ATile* Tile, UBuilding* Building)
{
	ClaimedTiles.Add(Tile);
	S_RefreshBorderingUnclaimedTiles();
	if (Building)
	{
		S_RegisterBuildingForIncome(Building);
	}
}

void ASettlement::RegisterPopulation(UPopulation* InPopulation)
{
	GetPopulation()->RegisterPop(InPopulation);
}

void ASettlement::S_UnregisterTile(ATile* Tile, UBuilding* Building)
{
	ClaimedTiles.Remove(Tile);
	S_RefreshBorderingUnclaimedTiles();
	if (Building)
	{
		S_UnregisterBuildingForIncome(Building);
	}
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

void ASettlement::C_AddResources(FConstructionResources Amount)
{
	Resources += Amount;
}

void ASettlement::C_RemoveResources(FConstructionResources Amount)
{
	Resources -= Amount;
}

int32 ASettlement::GetCountOfBuilders() const
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

TArray<ACivilian*> ASettlement::GetAllCivilians() const
{
	TArray<ACivilian*> Result;
	for (ATile* Tile : ClaimedTiles)
	{
		if (Tile && Tile->GetBuilding() && Tile->GetBuilding()->GetCivilian())
		{
			Result.Add(Tile->GetBuilding()->GetCivilian());
		}
	}
	return Result;
}

TArray<AArmy*> ASettlement::GetAllArmies() const
{
	TArray<AArmy*> Result;
	for (ATile* Tile : ClaimedTiles)
	{
		if (Tile && Tile->GetBuilding() && Tile->GetBuilding()->GetArmy())
		{
			Result.Add(Tile->GetBuilding()->GetArmy());
		}
	}
	return Result;
}

int32 ASettlement::GetCountOfConstructionSites() const
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

TArray<UBuilding*> ASettlement::GetAllBuildings() const
{
	TArray<UBuilding*> Result;
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		if (ClaimedTile && ClaimedTile->GetBuilding())
		{
			Result.Add(ClaimedTile->GetBuilding());
		}
	}
	return Result;
}

bool ASettlement::CanAddConstructionSite() const
{
	if (GetCountOfConstructionSites() >=
		GetCountOfBuilders() + ExtraAllowedConstructionSites)
	{
		return false;
	}
	// has free neighboring tiles
	TArray<ATile*> Tiles = BorderingUnclaimedTiles;
	for (int i = Tiles.Num() - 1; i >= 0; --i)
	{
		if (Tiles[i] && Tiles[i]->GetClaimant())
		{
			Tiles.RemoveAt(i);
		}
	}
	if (Tiles.Num() == 0)
	{
		return false;
	}
	return true;
}

void ASettlement::S_RegisterBuildingForIncome(UBuilding* Building)
{
	const EProductionType ProductionType = Building->GetProductionType();
	if (ProductionType == EProductionType::Food
		|| ProductionType == EProductionType::Wood
		|| ProductionType == EProductionType::Stone)
	{
		Production.Add(Building->GetProductionPerSecond(), ProductionType);
		Building->OnProductionPerSecondChanged.AddDynamic(this, &ASettlement::S_UpdateProduction);
	}
	Consumption.Add(Building->GetPopulation()->GetSize() * GetPopEatingPerSecond(), EResource::Food);
	Building->GetPopulation()->OnSizeChanged.AddDynamic(this, &ASettlement::S_UpdateConsumptionFromPopulation);
}

void ASettlement::S_UnregisterBuildingForIncome(UBuilding* Building)
{
	const EProductionType ProductionType = Building->GetProductionType();
	if (ProductionType == EProductionType::Food
		|| ProductionType == EProductionType::Wood
		|| ProductionType == EProductionType::Stone)
	{
		Production.Remove(Building->GetProductionPerSecond(), ProductionType);
		Building->OnProductionPerSecondChanged.RemoveDynamic(this, &ASettlement::S_UpdateProduction);
	}
	Consumption.Remove(Building->GetPopulation()->GetSize() * GetPopEatingPerSecond(), EResource::Food);
	Building->GetPopulation()->OnSizeChanged.RemoveDynamic(this, &ASettlement::S_UpdateConsumptionFromPopulation);
}

void ASettlement::S_UpdateProduction(const float Change, const float _, const EProductionType Type)
{
	Production.Add(Change, Type);
}

void ASettlement::S_UpdateConsumption(const float Change, const float _, const EResource Type)
{
	Consumption.Add(Change, Type);
}

void ASettlement::S_UpdateConsumptionFromPopulation(int16 Change)
{
	Consumption.Add(Change * GetPopEatingPerSecond(), EResource::Food);
}

// ----------------------- Logging -----------------------

TSharedPtr<FJsonObject> ASettlement::Log() const
{
	TSharedPtr<FJsonObject> NewLog = MakeShareable(new FJsonObject());
	// buildings
	int32 CountBuildings = GetAllBuildings().Num();
	int32 CountCivilianBuildings = 0;
	int32 CountArmyBuildings = 0;
	int32 CountDefenseBuildings = 0;
	for (UBuilding* Building : GetAllBuildings())
	{
		if (Cast<UBuildingCivilian>(Building))
			++CountCivilianBuildings;
		else if (Cast<UBuildingDefense>(Building))
			++CountDefenseBuildings;
		else if (Cast<UBuildingArmy>(Building))
			++CountArmyBuildings;
	}
	// log
	NewLog->SetNumberField(TEXT("CountBuildings"), CountBuildings);
	NewLog->SetNumberField(TEXT("CountCivilianBuildings"), CountCivilianBuildings);
	NewLog->SetNumberField(TEXT("CountArmyBuildings"), CountArmyBuildings);
	NewLog->SetNumberField(TEXT("CountDefenseBuildings"), CountDefenseBuildings);

	// Civilians
	int32 CountCivilians = GetAllCivilians().Num();
	int32 CountWoodcutter = 0;
	int32 CountForager = 0;
	int32 CountBuilder = 0;
	for (ACivilian* Civilian : GetAllCivilians())
	{
		if (Cast<AWoodcutter>(Civilian))
			++CountWoodcutter;
		else if (Cast<AForager>(Civilian))
			++CountForager;
		else if (Cast<ABuilder>(Civilian))
			++CountBuilder;
	}
	// log
	NewLog->SetNumberField(TEXT("CountCivilians"), CountCivilians);
	NewLog->SetNumberField(TEXT("CountWoodcutter"), CountWoodcutter);
	NewLog->SetNumberField(TEXT("CountForager"), CountForager);
	NewLog->SetNumberField(TEXT("CountBuilder"), CountBuilder);

	// Armies
	NewLog->SetNumberField(TEXT("Armies"), GetAllArmies().Num());

	// Pop
	NewLog->SetNumberField(TEXT("Pop"), Population->GetSize());

	// Resources
	NewLog->SetNumberField(TEXT("Food"), GetResources().Food);
	NewLog->SetNumberField(TEXT("Wood"), GetResources().Wood);
	NewLog->SetNumberField(TEXT("Stone"), GetResources().Stone);
	NewLog->SetNumberField(TEXT("FoodIncome"), GetEffectiveProduction().Food);
	NewLog->SetNumberField(TEXT("WoodIncome"), GetEffectiveProduction().Wood);
	NewLog->SetNumberField(TEXT("StoneIncome"), GetEffectiveProduction().Stone);

	return NewLog;
}
