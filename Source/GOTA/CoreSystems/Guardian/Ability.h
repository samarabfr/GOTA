// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTarget.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

UCLASS()
class GOTA_API AAbility : public AActor
{
	GENERATED_BODY()
	
	// ------------------------------------ Replication Setup --------------------------------------
	
	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	AAbility();
	
	// ---------------------------------------- Utility ----------------------------------------
public:
	virtual void SRPC_ActivateAbility(FAbilityTarget Target) { }

	virtual bool IsValidTarget(FAbilityTarget Target) { return false; }
};
