// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameBalance.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "CombatSystem.generated.h"

UCLASS(Blueprintable)
class GOTA_API UCombatSystem : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UCombatSystem();

	UPROPERTY()
	TSet<ATile*> CurrentCombatSources;

	UPROPERTY()
	UGameBalanceDataAsset* GameBalance;

public:
	void RegisterCombat(ATile* Tile);
	void TriggerAllCombats();

private:
	void EvaluateCombat(ATile* Tile);
	void CalcCombatValues(ATile* Tile, int32& Attack, int32& Defense, EAffiliation Affiliation);
	void DealDamage(ATile* Tile, int32 Damage, EAffiliation DamageReceiver);
};
