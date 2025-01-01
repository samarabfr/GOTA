// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

UCLASS()
class GOTA_API AAbility : public AActor
{
	GENERATED_BODY()

public:
	// Activates the Ability, returns false if the ability could not be activated for any reason. For example when
	// no target was selected first.
	virtual bool ActivateAbility() { return false; }
};
