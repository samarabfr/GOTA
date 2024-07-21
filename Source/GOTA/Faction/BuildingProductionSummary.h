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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UBuildingProductionSummary();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnythingChangedSignature);
	
public:
	UPROPERTY(BlueprintAssignable, Category="Building")
	FOnAnythingChangedSignature OnChanged;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<EProductionType, int32> ProductionMap;

	UFUNCTION()
	void RegisterBuildingProduction(UBuildingProduction* BuildingProduction);

	UFUNCTION()
	void UpdateBuildingProduction(int32 Change, EProductionType Type);
};