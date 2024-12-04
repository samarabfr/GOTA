#pragma once

#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
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

protected:
	UBuilding* GetBuilding() const { return Building.Get(); }
	UCivilianSettings* GetSettings() const { return Settings; }
	UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	// ----------------- State tree ------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	UStateTreeCivilianComponent* StateTree;

	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

public:
	float GetProgress() const { return Progress; }
	void SetProgress(float NewProgress);
	void AddProgress(float AddedProgress) { SetProgress(Progress + AddedProgress); }
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

public:
	virtual void S_Work();
	float GetWorkRate() const;
	bool IsCurrentTileValidForWork() const;
	bool IsPriorityTileValidForWork() const;
	bool TryFindPathToNearestTileValidForWork();
	ATile* GetPriorityTile() const { return PriorityTile.Get(); }
	void SetPriorityTile(ATile* NewPriorityTile) { PriorityTile = NewPriorityTile; }
	bool TryFindPathToPriorityTile();

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

public:
	void S_MoveToNextTileOnPath();
	float GetMovementRate() const { return MovementRate; };
	bool IsPathValid();
	bool IsPathEmpty() const { return Path.IsEmpty(); }
};
