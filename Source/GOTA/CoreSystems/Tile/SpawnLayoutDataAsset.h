// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagRule.h"
#include "SpawnBias.h"
#include "SpawnPoint.h"
#include "SpawnLayout.h"
#include "Engine/DataAsset.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "SpawnLayoutDataAsset.generated.h"

UCLASS()
class GOTA_API USpawnLayoutDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	USpawnLayoutDataAsset();
	
public:
	UPROPERTY(EditAnywhere)
	FName Name;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTagRule> GameplayTagRules;

	UPROPERTY(EditDefaultsOnly)
	bool GuaranteedIfPossible = false;
	
	UPROPERTY(EditDefaultsOnly)
	FSpawnBias SpawnBias;
	
	UPROPERTY(EditDefaultsOnly)
	FSpawnLayout SpawnLayout;

	bool IsValidFor(const FGameplayTagContainer& GameplayTagContainer);
};