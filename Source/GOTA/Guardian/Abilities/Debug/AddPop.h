// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/Guardian/Ability.h"
#include "AddPop.generated.h"

UCLASS()
class GOTA_API AAddPop : public AAbility
{
	GENERATED_BODY()

	virtual void S_ActivateAbility(FAbilityTarget Target) override;

	virtual bool IsValidTarget(FAbilityTarget Target) override;
};