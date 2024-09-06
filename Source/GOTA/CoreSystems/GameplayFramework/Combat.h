// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTile.h"
#include "CombatValues.h"
#include "GameBalance.h"
#include "Combat.generated.h"

UCLASS()
class GOTA_API ACombat : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ACombat();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MainMesh;

	UPROPERTY()
	UGameBalanceDataAsset* GameBalance;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SmallCombatMesh;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FCombatTile> CombatTiles;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAlliedAttack)
	int32 AlliedAttack = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAlliedDefense)
	int32 AlliedDefense = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetEnemyAttack)
	int32 EnemyAttack = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetEnemyDefense)
	int32 EnemyDefense = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAlliedDamage)
	int32 AlliedDamage = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetEnemyDamage)
	int32 EnemyDamage = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAlliedPopKills)
	int32 AlliedPopKills = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAlliedBuildingKills)
	int32 AlliedBuildingKills = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetEnemyPopKills)
	int32 EnemyPopKills = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetEnemyBuildingKills)
	int32 EnemyBuildingKills = 0;

	bool DoesCombatTilesContain(ATile* Tile);

	void AddCombatTile(FCombatTile CombatTile);

	void RemoveCombatTile(FCombatTile CombatTile);

public:
	void AddSource(ATile* Tile);

	bool ShouldMerge(ATile* Tile);

private:
	UFUNCTION()
	void EntityChanged(ATile* Tile, AEntity* OldEntity);

	UFUNCTION()
	void BuildingChanged(ATile* Tile);

	UFUNCTION()
	void CombatValuesChanged(UCombatValues* CombatValues);

	void CalcKills();

	bool PreventCalcKills = false;

	void CalcAttackDefense();

	void SpreadDamage(int32 Damage, EAffiliation Receiver);

	void SpreadDamageToEntities(EAffiliation Receiver, int32& DamageLeft);

	void SpreadDamageToBuildingPop(EAffiliation Receiver, int32& DamageLeft);

	void SpreadDamageToBuildings(EAffiliation Receiver, int32& DamageLeft);

	void CountKills();

public:
	void TriggerCombat();

private:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	// ---------------------------------------------------------
	// Getter & Setter
public:
	UFUNCTION(BlueprintGetter)
	int32 GetAlliedAttack() const;

	UFUNCTION(BlueprintGetter)
	int32 GetAlliedDefense() const;

	UFUNCTION(BlueprintGetter)
	int32 GetEnemyAttack() const;

	UFUNCTION(BlueprintGetter)
	int32 GetEnemyDefense() const;

	UFUNCTION(BlueprintGetter)
	int32 GetAlliedDamage() const;

	UFUNCTION(BlueprintGetter)
	int32 GetEnemyDamage() const;

	UFUNCTION(BlueprintGetter)
	int32 GetAlliedPopKills() const;

	UFUNCTION(BlueprintGetter)
	int32 GetAlliedBuildingKills() const;

	UFUNCTION(BlueprintGetter)
	int32 GetEnemyPopKills() const;

	UFUNCTION(BlueprintGetter)
	int32 GetEnemyBuildingKills() const;
};
