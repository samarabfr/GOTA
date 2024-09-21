// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"

#include "Building.generated.h"

class ASettlement;
class ATile;
class UPopulation;
class ACivilian;
class UBuildingDataAsset;

UCLASS(Blueprintable)
class GOTA_API UBuilding : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UBuilding();

public:
	void ServerInit(UBuildingDataAsset* DataAsset_, ATile* Tile_, ASettlement* Settlement_);
	
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingDataAsset* Settings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;

	UPROPERTY()
	ATile* Tile;

	UPROPERTY()
	ASettlement* Settlement;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionChangedSig, float, Change, EProductionType, ProductionType);

	FOnProductionChangedSig OnProductionChanged;
	
	float GetCurrentProduction() const;

	UFUNCTION()
	void PopSizeChanged(int16 Change);

	//---------------------Civilian Entity----------------
	
	UPROPERTY(VisibleInstanceOnly)
	ACivilian* Civilian;
	
	float GetCivilianWorkRate() const;
	float GetCivilianMovementRate() const;
	
	//---------------------Construction phase----------------
private:
	UPROPERTY(VisibleInstanceOnly)
	bool IsUnderConstruction;
public:
	bool GetIsUnderConstruction() const { return IsUnderConstruction; }
	FGameplayTag UnderConstructionTag;

private:
	UPROPERTY(VisibleInstanceOnly)
	FGameResources ResourceProgress;
public:
	FGameResources GetResourceProgress() const;
	void SetResourceProgress(const FGameResources NewResourcesProgress);
};
