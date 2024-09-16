// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Tile/GameplayTagRule.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "BuildingDataAsset.generated.h"

UCLASS()
class GOTA_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UBuildingDataAsset();
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FText Description;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	TArray<FGameplayTagRule> PlacementRules;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FGameResources Cost;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 Housing;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	EProductionType ProductionType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	float ProductionRate;
};
