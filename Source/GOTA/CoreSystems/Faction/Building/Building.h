// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingTierData.h"
#include "PopulationContainer.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Building.generated.h"

class UBuildingDataAsset;

UCLASS(Blueprintable)
class GOTA_API UBuilding : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UBuilding();
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionChangedSignature, int32, Changed, EProductionType, Type);

public:
	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category="Building")
	UBuildingDataAsset* DataAsset;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Instanced, Category="Building")
	UPopulationContainer* PopContainer;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category="Building")
	int32 Tier = 1;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Building")
	bool CanUpgrade();
	
	void Upgrade();

	// ---------------------------------------------------------
	// Production
	
	UPROPERTY(BlueprintAssignable, Category="Building")
	FOnProductionChangedSignature OnProductionChanged;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building")
	int32 Production = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building")
	int32 PopulationThreshold = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building")
	int32 ProductionPerThreshold = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category="Building")
	EProductionType ProductionType = EProductionType::MAX;
	
	UFUNCTION()
	void UpdateProduction(FPopulation Change);
	
	void SetupProduction(const FBuildingTierData* TierData);

	UCombatValues* GetCombatValues();
};
