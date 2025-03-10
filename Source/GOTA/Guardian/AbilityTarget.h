// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityTarget.generated.h"

class AArmy;
class ACivilian;
class AGuardian;
class ATile;

USTRUCT()
struct FAbilityTarget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<ATile> Tile;
	
	UPROPERTY()
	TWeakObjectPtr<AGuardian> Guardian;
	
	UPROPERTY()
	TWeakObjectPtr<ACivilian> Civilian;
	
	UPROPERTY()
	TWeakObjectPtr<AArmy> Army;
};
