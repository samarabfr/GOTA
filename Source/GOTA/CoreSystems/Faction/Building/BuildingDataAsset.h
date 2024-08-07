// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingTierData.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

UCLASS()
class GOTA_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	EFaction FactionStyle;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	TSubclassOf<class UBuilding> BuildingClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FBuildingTierData TierOne;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FBuildingTierData TierTwo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	FBuildingTierData TierThree;
	
	FBuildingTierData* GetTierData(int32 Tier);
};
