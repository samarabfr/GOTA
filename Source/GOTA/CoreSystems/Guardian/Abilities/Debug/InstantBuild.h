// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "InstantBuild.generated.h"

UCLASS()
class GOTA_API AInstantBuild : public AAbility
{
	GENERATED_BODY()

	virtual void SRPC_ActivateAbility(FAbilityTarget Target) override;

	virtual bool IsValidTarget(FAbilityTarget Target) override;
};