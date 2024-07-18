// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingProduction.h"
#include "BuildingProductionSummary.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UBuildingProductionSummary : public UObject
{
	GENERATED_BODY()
	UBuildingProductionSummary();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnythingChangedSignature);
	
public:
	FOnAnythingChangedSignature OnChanged;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<EProductionType, int32> ProductionMap;

	UFUNCTION()
	void RegisterBuildingProduction(UBuildingProduction* BuildingProduction);

	UFUNCTION()
	void UpdateBuildingProduction(int32 Change, EProductionType Type);
};