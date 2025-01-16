#pragma once

#include <functional>

#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Civilian.generated.h"

class UStateTreeCivilianComponent;
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

private:
	virtual void BeginDestroy() override;

public:
	UFUNCTION()
	void S_HandleDeath();

	// ----------------------- Utility -----------------------

private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<UBuilding> Building;

	UPROPERTY(Replicated)
	UCivilianSettings* Settings;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleInstanceOnly)
	UStateTreeCivilianComponent* StateTree;

protected:
	UBuilding* GetBuilding() const { return Building.Get(); }
	UCivilianSettings* GetSettings() const { return Settings; }
	AGS_Ingame* GetGameState()const { return GameState; }
	UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
	UStateTreeCivilianComponent* GetStateTree() const { return StateTree; }


	// ----------------- Progresser ------------------------
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

	UPROPERTY(EditAnywhere, Replicated)
	TWeakObjectPtr<ATile> PriorityTile;

protected:
	virtual bool IsTileValidForWork(const ATile* Tile) const;
	int32 GetWorkAmount() const { return WorkAmount; }
	bool S_TryFindPathToClosestWorkTile();
	bool S_TryFindPathToWorkTileClosestToSettlement();

public:
	virtual void S_Work();
	virtual bool S_TryFindPathToBestWorkTile();
	bool S_TryFindPathToPriorityTile();
	virtual bool IsCurrentTileAmongBestWorkTiles();
	bool IsCurrentTilePriorityTile();
	float GetWorkRate() const;
	ATile* GetPriorityTile() const { return PriorityTile.Get(); }
	void S_SetPriorityTile(ATile* NewPriorityTile) { PriorityTile = NewPriorityTile; }

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
