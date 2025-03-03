// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnLayoutRegisterEntry.generated.h"

class ASpawnLayoutActor;

/**
 * Base class for SpawnLayoutRegister Datatables 
 */
USTRUCT()
struct GOTA_API FSpawnLayoutRegisterEntry : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ASpawnLayoutActor* SpawnLayoutActor;
};
