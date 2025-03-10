#include "Civilian.h"
#include "GOTA/Tile/Building/Building.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/Utility/ResourceStorage.h"
#include "GOTA//Settlement/Settlement.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tilemap/TileMap.h"
#include "Net/UnrealNetwork.h"

// ----------------------- Replication Setup -----------------------

void ACivilian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, WorkAmount, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, PriorityTile, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Storage, Params);
}

// ----------------------- LifeCycle -----------------------

ACivilian::ACivilian()
{
	Storage = CreateDefaultSubobject<UResourceStorage>("Storage");
}

void ACivilian::S_Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	Super::S_Init(InBuilding, SpawnTile);

	FVector NewLocation = FVector();
	SpawnTile->AddCivilian(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = GetOriginBuilding()->GetSettings();
	WorkAmount = BuildingSettings->CivilianGatheringAmount;
	S_SetMovementRate(100 / BuildingSettings->CivilianMoveTime);
	Storage->S_SetLimit(BuildingSettings->CivilianStorageLimit +
		GetOriginBuilding()->GetEfficiency() *
		GetOriginBuilding()->GetSettings()->CivilianStorageLimitIncreasePerEfficiencyPercentage);
}

// ----------------- Utility ------------------------

EEntityType ACivilian::GetEntityType() const
{
	return EEntityType::Civilian;
}

// ----------------- Working ------------------------

bool ACivilian::IsTileValidForWork(const ATile* Tile) const
{
	return false;
}

int32 ACivilian::GetWorkAmount() const
{
	return WorkAmount;
}

void ACivilian::S_Work()
{
}

bool ACivilian::S_TryFindPathToBestWorkTile()
{
	return S_TryFindPathToClosestWorkTile();
}

bool ACivilian::IsCurrentTileAmongBestWorkTiles()
{
	return IsTileValidForWork(GetCurrentTile());
}

bool ACivilian::IsCurrentTilePriorityTile() const
{
	return GetCurrentTile() == GetPriorityTile();
}

float ACivilian::GetWorkRate() const
{
	return 100.f / GetOriginBuilding()->GetSettings()->CivilianGatheringTime * GetOriginBuilding()->GetEfficiency();
}

ATile* ACivilian::GetPriorityTile() const
{
	return PriorityTile.Get();
}

void ACivilian::S_SetPriorityTile(ATile* NewPriorityTile)
{
	PriorityTile = NewPriorityTile;
}

bool ACivilian::HasResourcesInInventory() const
{
	return Storage->GetCurrent() > 0;
}

bool ACivilian::IsInventoryFull() const
{
	return Storage->IsFull();
}

bool ACivilian::IsCurrentTileOriginBuilding() const
{
	if (!GetCurrentTile() || !GetOriginBuilding() || !GetOriginBuilding()->GetTile()) return false;
	return GetCurrentTile() == GetOriginBuilding()->GetTile();
}

void ACivilian::S_UnloadResources()
{
	GetOriginBuilding()->GetResourceStorage()->S_Add(Storage->GetCurrent());
	Storage->S_Empty();
}

UResourceStorage* ACivilian::GetStorage() const
{
	return Storage;
}

bool ACivilian::S_TryFindPathToClosestWorkTile()
{
	if (!GetCurrentTile() || !S_GetGameState()->GetTileMap()) return false;
	if (IsTileValidForWork(GetCurrentTile())) return true;
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Civilian,
		[this](const ATile* Tile)
		{
			return IsTileValidForWork(Tile);
		});
	if (ResultPath.IsEmpty()) return false;
	S_SetPath(ResultPath);
	return true;
}

bool ACivilian::S_TryFindPathToWorkTileClosestToSettlement()
{
	if (!GetOriginBuilding() ||
		!GetOriginBuilding()->GetSettlement() ||
		GetOriginBuilding()->GetSettlement()->ClaimedTiles.IsEmpty() ||
		!S_GetGameState()->GetTileMap())
		return false;
	for (ATile* ClaimedTile : GetOriginBuilding()->GetSettlement()->ClaimedTiles)
	{
		if (IsTileValidForWork(ClaimedTile))
		{
			TArray<ATile*> Origin;
			Origin.Add(GetCurrentTile());
			const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToTile(Origin, ClaimedTile);
			if (!ResultPath.IsEmpty())
			{
				S_SetPath(ResultPath);
				return true;
			}
		}
	}
	TArray<ATile*> PathOrigin;
	PathOrigin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTileFromSearchOrigin(
		GetOriginBuilding()->GetSettlement()->ClaimedTiles, PathOrigin
		, EEntityType::Civilian,
		[this](const ATile* Tile)
		{
			return IsTileValidForWork(Tile);
		});
	if (ResultPath.IsEmpty()) return false;
	S_SetPath(ResultPath);
	return true;
}

void ACivilian::S_HandleEfficiencyChange(float Change)
{
	if (!GetOriginBuilding())
		return;
	Storage->S_SetLimit(GetOriginBuilding()->GetSettings()->CivilianStorageLimit +
		GetOriginBuilding()->GetEfficiency() *
		GetOriginBuilding()->GetSettings()->CivilianStorageLimitIncreasePerEfficiencyPercentage);
}

bool ACivilian::S_TryFindPathToPriorityTile()
{
	if (!GetCurrentTile() ||
		!PriorityTile.IsValid() ||
		!S_GetGameState()->GetTileMap())
	{
		return false;
	}
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToTile(Origin, GetPriorityTile(),
		EEntityType::Civilian);
	if (ResultPath.IsEmpty()) return false;
	S_SetPath(ResultPath);
	return true;
}

bool ACivilian::S_TryFindPathToOriginBuilding()
{
	if (!GetCurrentTile() ||
		!GetOriginBuilding() ||
		!GetOriginBuilding()->GetTile() ||
		!S_GetGameState()->GetTileMap())
	{		
		return false;
	}
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToTile(
		Origin, GetOriginBuilding()->GetTile(),
		EEntityType::Civilian);
	if (ResultPath.IsEmpty()) return false;
	S_SetPath(ResultPath);
	return true;
}
