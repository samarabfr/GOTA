// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SimplifiedAbility.h"

#include "DamageBuildingAbility.generated.h"


class ATile;

UCLASS(Blueprintable)
class GOTA_API ADamageBuildingAbility : public ASimplifiedAbility
{
	GENERATED_BODY()

	virtual void Use(ATile* Target, ATile* PlayerPosition) override;
};
