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
	bool IsTargetValid() const;

public:
	virtual void CalculateMovement() override;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Instanced)
	UPopulationContainer* PopCon;

	virtual int32 GetAttack() const override;

	virtual int32 GetDefense() const override;
};
