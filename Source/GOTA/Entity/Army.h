// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Entity.h"
#include "GOTA/Utility/Enums.h"
#include "Army.generated.h"

class UStateTreeComponent;
class UCombatValues;
class UBuilding;
class AGS_Ingame;
class ATile;
class ASettlement;
class UArmySettings;

UCLASS()
class GOTA_API AArmy : public AEntity
{
	GENERATED_BODY()

	// ----------------------- Replication Setup -----------------------
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ----------------------- LifeCycle -----------------------
protected:
	AArmy();

public:
	virtual void S_Init(UBuilding* InBuilding, ATile* SpawnTile) override;
	virtual void S_Tick(const float DeltaSeconds) override;

	// ----------------------- Utility -----------------------

private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ColonyArmyMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* NativeArmyMesh;

	virtual void OnRep_Affiliation() override;
	void RefreshMesh();

public:
	virtual EEntityType GetEntityType() const override;

	// ----------------- Mode ------------------------
private:
	UPROPERTY(EditInstanceOnly, Replicated)
	EArmyMode Mode = EArmyMode::GarrisonMode;

	UPROPERTY(VisibleInstanceOnly)
	float GoAttackModeTimeLeft = 0.0f;

public:
	EArmyMode GetMode() const;
	void SetMode(EArmyMode NewMode);

	// ----------------- Recruiting ------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float RecruitRate; // in percent per second

	bool IsTileValidForRecruiting(const ATile* Tile) const;

public:
	float GetRecruitRate() const;
	void S_TakePopFromTile();
	bool IsCurrentTileValidForRecruiting() const;
	bool S_TryFindPathToNearestRecruitable();

	// -----------------Combat------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UCombatValues* CombatValues;

	UPROPERTY(EditDefaultsOnly)
	int32 GarrisonModeInterceptingRange = 4;

public:
	UCombatValues* GetCombatValues() const;
	TArray<AArmy*> GetNeighboringEnemyArmies() const;
	TArray<UBuilding*> GetNeighboringEnemyDefenseBuildings() const;
	bool HasEnemyOnNeighboringTile() const;
	bool S_TryFindPathToNearestEnemy();
	bool S_TryFindPathToNearestEnemyUnprotectedNormalBuilding();
	bool S_TryFindPathToNearestEnemyDefenseBuilding();
	bool HasEnemyInGarrisonModeRange() const;
	void S_ArmyTakeDamage(int32 Damage);
	void S_AttackEnemy();

	// -----------------Ravaging------------------------

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float RavageSpeed; // in percent per second

public:
	bool IsOnEnemyBuilding() const;
	bool IsBuildingProtected() const;
	float GetRavageSpeed() const;
	void S_RavageEnemyBuilding();

	// -----------------Guarding------------------------

private:
	UPROPERTY(EditInstanceOnly, Replicated)
	TWeakObjectPtr<ATile> GuardTile;

public:
	ATile* GetGuardTile() const;
	void S_SetGuardTile(ATile* NewGuardTile);
	bool IsOnGuardTile() const;
	bool S_TryFindPathToGuardTile();
	bool S_TryFindPathToNearestEnemyToGuardTile();
	bool HasEnemyInGuardTileRange();

	// -----------------Intercepting------------------------

private:
	UPROPERTY(EditInstanceOnly, Replicated)
	TWeakObjectPtr<AArmy> InterceptArmy;

	UPROPERTY(EditDefaultsOnly)
	int32 GuardModeInterceptingRange = 3;

public:
	TWeakObjectPtr<AArmy> GetInterceptArmy() const;
	void S_SetInterceptArmy(TWeakObjectPtr<AArmy> NewInterceptArmy);
	bool S_TryFindPathToInterceptArmy();
};
