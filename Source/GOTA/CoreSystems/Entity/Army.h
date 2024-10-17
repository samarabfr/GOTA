// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "Army.generated.h"

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
	
	UPROPERTY(VisibleInstanceOnly)
	int32 Size = 0;



	// -----------------Status------------------------

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	EArmyStatus Status = EArmyStatus::Idle;

protected:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	virtual void ValidateStatus();
	
	bool IsTileValidForRecruiting(const ATile* Tile) const;

	EArmyStatus GetStatus() const { return Status; }
	void SetStatus(EArmyStatus NewStatus);
	
	// ----------------- Recruiting ------------------------

protected:
	UPROPERTY(VisibleInstanceOnly)
	float RecruitRate; // in percent per second
	
	bool TryFindNearestRecruitable();
	void TakePopFromTile();

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
