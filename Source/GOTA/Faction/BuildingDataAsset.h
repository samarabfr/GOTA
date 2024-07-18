// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

/**
 * 
 */
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
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	int32 WoodCost;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	TSubclassOf<class UBuilding> BuildingClass;
};
