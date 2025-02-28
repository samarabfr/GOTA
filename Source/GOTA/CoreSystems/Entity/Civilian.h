#pragma once

#include <functional>

#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Civilian.generated.h"

class UStateTreeComponent;
class AGS_Ingame;
class ATile;
class ASettlement;
class UCivilianSettings;
class UBuilding;

UCLASS()
class GOTA_API ACivilian : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ----------------------- LifeCycle -----------------------

protected:
	ACivilian();

public:
	void S_Init(UBuilding* InBuilding, ATile* SpawnTile);

	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

	void Delete();

private:
	virtual void BeginDestroy() override;

public:
	UFUNCTION()
	void S_HandleDeath();

	// ----------------------- Utility -----------------------

private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<UBuilding> OriginBuilding;

	UPROPERTY()
	TWeakObjectPtr<AGS_Ingame> GameState;

protected:
	UPROPERTY(EditDefaultsOnly)
	UStateTreeComponent* StateTree;
	
	UBuilding* GetOriginBuilding() const { return OriginBuilding.Get(); }
	AGS_Ingame* S_GetGameState() const { return GameState.Get(); }

	// ----------------- Progresser ------------------------
protected:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bProgresserActive = false;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float ProgressRate = 0.f;

	std::function<float()> CalculateProgressRate;
	std::function<void()> FinishProgress;

public:
	void S_StartProgresser(const std::function<float()>& ProgressRateCalculator,
	                       const std::function<void()>& Finisher);
	void S_StopProgresser();
	void ProgressTick(float DeltaSeconds);

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
	virtual bool IsTileValidForWork(const ATile* Tile) const;
	int32 GetWorkAmount() const { return WorkAmount; }
	UResourceStorage* GetStorage() const { return Storage; }
	bool S_TryFindPathToClosestWorkTile();
	bool S_TryFindPathToWorkTileClosestToSettlement();

	UFUNCTION()
	void HandleEfficiencyChange(float Change);

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

	// ----------------- Moving ------------------------
private:
	UPROPERTY(ReplicatedUsing=OnRep_NetLocation)
	FVector NetLocation;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TWeakObjectPtr<ATile> CurrentTile;

	UFUNCTION()
	void OnRep_NetLocation();

	void SetNetLocation(const FVector& NewNetLocation);

	// How fast the progress increases when moving, in percent per second
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float MovementRate;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

protected:
	ATile* GetCurrentTile() const { return CurrentTile.Get(); }
	void S_SetPath(const TArray<ATile*>& NewPath) { Path = NewPath; }

public:
	void S_MoveToNextTileOnPath();
	float GetMovementRate() const { return MovementRate; };
	bool IsPathValid();
	bool IsPathEmpty() const { return Path.IsEmpty(); }
};
