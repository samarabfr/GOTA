// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "Army.generated.h"

class UStateTreeComponentArmy;
class UBuilding;
class AGS_Ingame;
class ATile;
class ASettlement;
class UArmySettings;

UCLASS()
class GOTA_API AArmy : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	AArmy();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void S_Init(UBuilding* InBuilding, ATile* SpawnTile);

	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;

	// -----------------------  -----------------------

protected:
	UPROPERTY(Replicated)
	UBuilding* Building;

	UPROPERTY(Replicated)
	UArmySettings* Settings;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Affiliation)
	EAffiliation Affiliation;

public:
	EAffiliation GetAffiliation() const;

private:
	UFUNCTION()
	void OnRep_Affiliation();

	UPROPERTY(VisibleInstanceOnly, Replicated)
	int32 Size = 0;

	// ----------------- Status ------------------------
private:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	EArmyStatus Status = EArmyStatus::Idling;
	
	UPROPERTY()
	UStateTreeComponentArmy* StateTree;

protected:
	void SetStatus(EArmyStatus NewStatus);
	
public:

	float GetProgress() const { return Progress; }
	EArmyStatus GetStatus() const { return Status; }

	// ----------------- Recruiting ------------------------

protected:
	UPROPERTY(VisibleInstanceOnly)
	float RecruitRate; // in percent per second
	
	bool IsTileValidForRecruiting(const ATile* Tile) const;
	bool TryFindNearestRecruitable();
	void TakePopFromTile();

public:
	void RecruitFromTile();

	// ----------------- Moving ------------------------

protected:
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
	float MovementRate; // in percent per second

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

private:
	void MoveToNextTileOnPath();

	// -----------------Combat------------------------
private:
	void CheckForCombat();
	void InitializeCombat(AArmy* Enemy);
	void JoinCombat(AArmy* Enemy);

public:
	void ChallengeToCombat();
};
