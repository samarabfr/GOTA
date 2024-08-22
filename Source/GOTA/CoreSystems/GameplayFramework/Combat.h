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

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MainMesh;
	
	UPROPERTY()
	UGameBalanceDataAsset* GameBalance;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SmallCombatMesh;
	
	UPROPERTY(VisibleInstanceOnly)
	TArray<FCombatTile> CombatTiles;

	UPROPERTY(VisibleInstanceOnly)
	int32 AlliedAttack = 0;

	UPROPERTY(VisibleInstanceOnly)
	int32 AlliedDefense = 0;

	UPROPERTY(VisibleInstanceOnly)
	int32 EnemyAttack = 0;

	UPROPERTY(VisibleInstanceOnly)
	int32 EnemyDefense = 0;

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
public:
	void TriggerCombat();

private:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;
};
