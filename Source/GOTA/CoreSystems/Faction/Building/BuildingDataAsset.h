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

	//------------------Income------------------
	
	UPROPERTY(EditDefaultsOnly, Category="Income")
	EProductionType IncomeType;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	float IncomeTime;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	float IncomeAmount;

	//--------------------------Civilian Entity-------------------
	
	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	TSubclassOf<ACivilian> CivilianClass;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float SecondsPerWorkCycle;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	int32 WorkAmountPerCycle;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float SecondsPerMove;	
};
