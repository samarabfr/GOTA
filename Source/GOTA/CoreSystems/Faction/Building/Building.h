// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void ServerInit(UBuildingDataAsset* DataAsset_, ATile* Tile, ASettlement* Settlement);
	
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingDataAsset* Settings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionChangedSig, float, Change, EProductionType, ProductionType);

	FOnProductionChangedSig OnProductionChanged;
	
	float GetCurrentProduction() const;

	UFUNCTION()
	void PopSizeChanged(int16 Change);

	//---------------------Civilian Entity----------------
	
	UPROPERTY()
	ACivilian* Civilian;
	
	float GetCivilianWorkRate() const;
	float GetCivilianMovementRate() const;
};
