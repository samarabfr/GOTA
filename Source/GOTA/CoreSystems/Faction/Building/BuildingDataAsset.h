// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Tile/GameplayTagRule.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "BuildingDataAsset.generated.h"

class ACivilian;

UCLASS()
class GOTA_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UBuildingDataAsset();
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Building")
	FName Name;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, Category="Building")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	TArray<FGameplayTagRule> PlacementRules;
	
	UPROPERTY(EditDefaultsOnly, Category="Building")
	FGameResources Cost;
	
	UPROPERTY(EditDefaultsOnly, Category="Building")
	int32 Housing;

	//------------------Production Per Time Per Pop------------------
	
	UPROPERTY(EditDefaultsOnly, Category="Production Per Time Per Pop")
	EProductionType ProductionType;

	UPROPERTY(EditDefaultsOnly, Category="Production Per Time Per Pop")
	float ProductionRate;

	//--------------------------Civilian Entity-------------------
	
	UPROPERTY(EditDefaultsOnly, Category="Civilian Entity")
	TSubclassOf<ACivilian> CivilianEntityClass;

	UPROPERTY(EditDefaultsOnly, Category="Civilian Entity")
	float SecondsPerCycle;

	UPROPERTY(EditDefaultsOnly, Category="Civilian Entity")
	int16 ProductionPerCycle;	
};
