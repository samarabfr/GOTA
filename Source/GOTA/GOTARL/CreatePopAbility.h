// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SimplifiedAbility.h"

#include "CreatePopAbility.generated.h"


class ATile;

UCLASS(Blueprintable)
class GOTA_API ACreatePopAbility : public ASimplifiedAbility
{
	GENERATED_BODY()
	
public:
	ACreatePopAbility();
	virtual void Use(ATile* Target, ATile* PlayerPosition) override;
};
