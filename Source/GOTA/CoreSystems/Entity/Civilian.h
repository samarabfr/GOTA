#pragma once

#include <functional>

#include "Entity.h"
#include "Civilian.generated.h"


class UResourceStorage;

UCLASS()
class GOTA_API ACivilian : public AEntity
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ----------------------- LifeCycle -----------------------

protected:
	ACivilian();

	UFUNCTION()
	virtual void S_HandleDeath() override;

public:
	virtual void S_Init(UBuilding* InBuilding, ATile* SpawnTile) override;

	// ----------------- Utility ------------------------
public:
	virtual EEntityType GetEntityType() const override;

	// ----------------- Working ------------------------
private:
	// How much impact the Work has, for example when producing resources how many resources get produced
	UPROPERTY(VisibleInstanceOnly, Replicated)
	int32 WorkAmount;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	UResourceStorage* Storage;

	UPROPERTY(EditAnywhere, Replicated)
	TWeakObjectPtr<ATile> PriorityTile;

protected:
	virtual void S_HandleEfficiencyChange(float Change) override;
	virtual bool IsTileValidForWork(const ATile* Tile) const;
	int32 GetWorkAmount() const { return WorkAmount; }
	UResourceStorage* GetStorage() const { return Storage; }
	bool S_TryFindPathToClosestWorkTile();
	bool S_TryFindPathToWorkTileClosestToSettlement();

public:
	virtual void S_Work();
	virtual bool S_TryFindPathToBestWorkTile();
	bool S_TryFindPathToPriorityTile();
	bool S_TryFindPathToOriginBuilding();
	virtual bool IsCurrentTileAmongBestWorkTiles();
	bool IsCurrentTilePriorityTile() const;
	float GetWorkRate() const;
	ATile* GetPriorityTile() const { return PriorityTile.Get(); }
	void S_SetPriorityTile(ATile* NewPriorityTile) { PriorityTile = NewPriorityTile; }
	bool HasResourcesInInventory() const;
	bool IsInventoryFull() const;
	bool IsCurrentTileOriginBuilding() const;
	void S_UnloadResources();
};
