// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameResources.h"
#include "SettlementSettings.generated.h"

class AArmy;
class UBuildingSettings;

UCLASS(Blueprintable)
class GOTA_API USettlementSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	FGameplayTagContainer C_GameplayTags;
	
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	TArray<UBuildingSettings*> C_PossibleBuildings;
	
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	TArray<UBuildingSettings*> C_StartingBuildings;
	
	UPROPERTY(EditDefaultsOnly, Category="Colonist Settlement")
	FGameResources C_StartingResources;

	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	FGameplayTagContainer N_GameplayTags;

	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	TArray<UBuildingSettings*> N_StartingBuildings;
	
	UPROPERTY(EditDefaultsOnly, Category="Native Settlement")
	FGameResources N_StartingResources;

	
	UPROPERTY(EditDefaultsOnly, Category="Eating and Starving")
	float PopEatingPerSecond;
	
	UPROPERTY(EditDefaultsOnly, Category="Eating and Starving")
	float StarvingThreshold;

		
	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float FoodImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float FoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float WoodImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float WoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float StoneImportance = 1;

	UPROPERTY(EditDefaultsOnly, Category="Importance Rating")
	float StoneImportanceDescent = 0.1;
	

	UPROPERTY(EditDefaultsOnly, Category="Colony brain")
	float SendArmiesIntervalTime = 60.0f;
};
