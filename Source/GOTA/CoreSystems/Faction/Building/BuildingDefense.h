#pragma once
#include "Building.h"

#include "BuildingDefense.generated.h"

class UCombatValues;

UCLASS(Blueprintable)
class GOTA_API UBuildingDefense : public UBuilding
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
private:
	UBuildingDefense();
public:
	virtual void S_Tick(float DeltaSeconds) override;
	virtual void C_Tick(const float DeltaSeconds) override;

protected:
	virtual void BeginDestroy() override;

	// -----------------Combat------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UCombatValues* CombatValues;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float AttackProgress;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bIsAttacking;

	void S_AttackEnemy();

	UFUNCTION()
	void S_HandleDeath();

public:
	UCombatValues* GetCombatValues() const { return CombatValues; }
	TArray<AArmy*> GetNeighboringEnemies() const;
	bool HasEnemyOnNeighboringTile() const;
	void S_StartAttacking();
	void S_BuildingDefenseTakeDamage(int32 Damage);
};
