// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"

#include "Building.generated.h"

class AArmy;
class ASettlement;
class ATile;
class UPopulation;
class ACivilian;
class UBuildingSettings;

UCLASS(Blueprintable)
class GOTA_API UBuilding : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UBuilding();
	
public:
	void ServerInit(UBuildingSettings* DataAsset_, ATile* Tile_, ASettlement* Settlement_);
	void GOTATick(float DeltaSeconds);
	
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingSettings* Settings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;

	UFUNCTION()
	void ProductionChanged(int16 Change);

	UPROPERTY()
	ATile* Tile;

	UPROPERTY()
	ASettlement* Settlement;

	//---------------------base income----------------

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIncomeChangedSig, float, IncomeChange, EProductionType, ProductionType);
	FOnIncomeChangedSig OnIncomeChanged;
	float GetCurrentIncomePerSecond() const;
	void AddIncomeToSettlement();
	
	UPROPERTY(VisibleInstanceOnly)
	float IncomeProgress = 0.0f;

	//---------------------Civilian Entity----------------
	
	UPROPERTY(VisibleInstanceOnly)
	ACivilian* Civilian;
	
	float GetCivilianWorkRate() const;
	float GetCivilianMovementRate() const;

	//---------------------Civilian Entity----------------
	
	UPROPERTY(VisibleInstanceOnly)
	AArmy* Army;
	
	float GetArmyRecruitRate() const;
	float GetArmyMovementRate() const;
	
	//---------------------Construction phase----------------
private:
	UPROPERTY(VisibleInstanceOnly)
	bool IsUnderConstruction;
public:
	bool GetIsUnderConstruction() const { return IsUnderConstruction; }

private:
	UPROPERTY(VisibleInstanceOnly)
	FGameResources ResourceProgress;
public:
	FGameResources GetResourceProgress() const;
	void SetResourceProgress(const FGameResources NewResourcesProgress);

	void FinishConstruction();
};
