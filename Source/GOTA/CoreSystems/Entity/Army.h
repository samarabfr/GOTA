// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "Army.generated.h"

class UCombatValues;
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

	// ----------------------- Utility -----------------------

private:
	UPROPERTY(Replicated)
	UBuilding* Building;

	UPROPERTY(Replicated)
	UArmySettings* Settings;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Affiliation)
	EAffiliation Affiliation;

	UFUNCTION()
	void OnRep_Affiliation();

	UPROPERTY(EditInstanceOnly, Replicated)
	EArmyMode Mode = EArmyMode::GarrisonMode;

public:
	EAffiliation GetAffiliation() const;
	EArmyMode GetMode() const;
	void SetMode(EArmyMode NewMode);

	// ----------------- Status ------------------------
private:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	EArmyStatus Status = EArmyStatus::Idling;

	UPROPERTY(VisibleInstanceOnly)
	UStateTreeComponentArmy* StateTree;

protected:
	void SetStatus(EArmyStatus NewStatus);

public:
	float GetProgress() const { return Progress; }
	EArmyStatus GetStatus() const { return Status; }

	// ----------------- Recruiting ------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float RecruitRate; // in percent per second

	void S_TakePopFromTile();
	bool IsTileValidForRecruiting(const ATile* Tile) const;

public:
	void S_StartRecruitFromTile();
	bool IsCurrentTileValidForRecruiting() const;
	bool TryFindPathToNearestRecruitable();

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
	float MovementRate; // in percent per second

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

public:
	bool IsPathValid();
	void S_StartMoveToNextTileOnPath();

	// -----------------Combat------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UCombatValues* CombatValues;

public:
	UCombatValues* GetCombatValues() const { return CombatValues; }
	TArray<AArmy*> GetNeighboringEnemyArmies() const;
	TArray<UBuilding*> GetNeighboringEnemyDefenseBuildings() const;
	bool HasEnemyOnNeighboringTile() const;
	bool TryFindPathToNearestEnemy();
	bool TryFindPathToNearestEnemyUnprotectedNormalBuilding();
	bool TryFindPathToNearestEnemyDefenseBuilding();
	bool HasEnemyInGarrisonModeRange() const;
	void S_StartAttacking();
	void S_ArmyTakeDamage(int32 Damage);

private:
	void S_AttackEnemy();

public:
	UFUNCTION()
	void S_HandleDeath();

	// -----------------Ravaging------------------------

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float RavageSpeed; // in percent per second

	void S_RavageEnemyBuilding();

public:
	bool IsOnEnemyBuilding() const;
	bool IsBuildingProtected() const;
	void S_StartRavagingEnemyBuilding();


	// -----------------Guarding------------------------

private:
	UPROPERTY(EditInstanceOnly, Replicated)
	ATile* GuardTile;

public:
	ATile* GetGuardTile() const { return GuardTile; }
	void SetGuardTile(ATile* NewGuardTile) { GuardTile = NewGuardTile; }
	bool IsOnGuardTile() const { return GuardTile == CurrentTile; }
	bool TryFindPathToGuardTile();
	bool TryFindPathToNearestEnemyToGuardTile();
	bool HasEnemyInGuardTileRange();

	// -----------------Intercepting------------------------

private:
	UPROPERTY(EditInstanceOnly, Replicated)
	TWeakObjectPtr<AArmy> InterceptArmy;

public:
	TWeakObjectPtr<AArmy>  GetInterceptArmy() const { return InterceptArmy; }
	void SetInterceptArmy(TWeakObjectPtr<AArmy>  NewInterceptArmy) { InterceptArmy = NewInterceptArmy; }
	bool TryFindPathToInterceptArmy();
};
