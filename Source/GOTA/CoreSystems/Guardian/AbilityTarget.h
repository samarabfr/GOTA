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
	TWeakObjectPtr<ATile> Tile;
	TWeakObjectPtr<AGuardian> Guardian;
	TWeakObjectPtr<ACivilian> Civilian;
	TWeakObjectPtr<AArmy> Army;
};
