// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SimplifiedAbility.h"

#include "DamageArmyAbility.generated.h"


class ATile;

UCLASS(Blueprintable)
class GOTA_API ADamageArmyAbility : public ASimplifiedAbility
{
	GENERATED_BODY()

protected:
	int32 Damage = 5;
	
public:
	ADamageArmyAbility();
	virtual void Use(ATile* Target, ATile* PlayerPosition) override;
};
