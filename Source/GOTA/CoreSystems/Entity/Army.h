// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationContainer.h"
#include "Army.generated.h"

UCLASS()
class GOTA_API AArmy : public AEntity
{
	GENERATED_BODY()
	AArmy();
	bool IsTargetValid();

public:
	virtual void CalculateMovement() override;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Instanced)
	UPopulationContainer* PopCon;

	virtual int32 GetAttack() const override;

	virtual int32 GetDefense() const override;

	virtual int32 GetHP() const override;

	virtual void DealDamage(int32 Damage) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_) override;
};
