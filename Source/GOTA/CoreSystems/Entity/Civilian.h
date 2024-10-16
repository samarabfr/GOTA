#pragma once

#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Civilian.generated.h"

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

	// -----------------------  -----------------------

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Building)
	UBuilding* Building;

	UFUNCTION()
	void OnRep_Building();

	UPROPERTY(Replicated)
	UCivilianSettings* Settings;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	// ----------------------- Status -----------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	ECivilianStatus Status;

protected:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	virtual void ValidateStatus();

	ECivilianStatus GetStatus() const { return Status; }
	void SetStatus(ECivilianStatus NewStatus);

	// ----------------- Working ------------------------

	// How fast the progress increases when working, in percent per second
	UPROPERTY(VisibleInstanceOnly)
	float WorkRate;

	// How much impact the Work has, for example when producing resources how many resources get produced
	UPROPERTY(VisibleInstanceOnly, Replicated)
	int32 WorkAmount;

private:
	void SetupPopSizeChanging();

	UFUNCTION()
	void OnPopSizeChanged(int16 Change);

	void CalculateWorkRate();

protected:
	virtual void Work();

	// ----------------- Moving ------------------------

	UPROPERTY(VisibleInstanceOnly, Replicated)
	ATile* CurrentTile;

private:
	UPROPERTY(ReplicatedUsing=OnRep_NetLocation)
	FVector NetLocation;

	UFUNCTION()
	void OnRep_NetLocation();

	void SetNetLocation(const FVector& NewNetLocation);

protected:
	// How fast the progress increases when moving, in percent per second
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float MovementRate;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

private:
	void Move();
};
