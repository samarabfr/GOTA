#pragma once

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
	UBuilding* Building;

	UPROPERTY(Replicated)
	UCivilianSettings* Settings;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

protected:
	UBuilding* GetBuilding() const { return Building; }
	UCivilianSettings* GetSettings() const { return Settings; }
	UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	// ----------------------- Status -----------------------
private:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	ECivilianStatus Status = ECivilianStatus::Idling;

	UPROPERTY(VisibleInstanceOnly)
	UStateTreeCivilianComponent* StateTree;

protected:
	ECivilianStatus GetStatus() const { return Status; }

public:
	float GetProgress() const { return Progress; }
	void SetStatus(ECivilianStatus NewStatus);

	// ----------------- Working ------------------------
private:
	// How fast the progress increases when working, in percent per second
	UPROPERTY(VisibleInstanceOnly)
	float WorkRate;

	// How much impact the Work has, for example when producing resources how many resources get produced
	UPROPERTY(VisibleInstanceOnly, Replicated)
	int32 WorkAmount;

protected:
	virtual void S_Work();
	virtual bool IsTileValidForWork(ATile* Tile) const;
	int32 GetWorkAmount() const { return WorkAmount; }

public:
	void S_StartWorking();
	bool IsCurrentTileValidForWork() const;

	// ----------------- Moving ------------------------
private:
	UPROPERTY(ReplicatedUsing=OnRep_NetLocation)
	FVector NetLocation;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	ATile* CurrentTile;

	UFUNCTION()
	void OnRep_NetLocation();

	void SetNetLocation(const FVector& NewNetLocation);
	void S_MoveToNextTileOnPath();

	// How fast the progress increases when moving, in percent per second
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float MovementRate;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

protected:
	ATile* GetCurrentTile() const { return CurrentTile; }

public:
	bool IsPathValid();
	void S_StartMoveToNextTileOnPath();
};
