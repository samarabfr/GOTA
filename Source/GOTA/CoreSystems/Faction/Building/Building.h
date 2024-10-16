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
	void ServerInit(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement);
	void ClientInit();
	
private:
	virtual void BeginDestroy() override;
	
public:
	void ServerTick(float DeltaSeconds);
	void ClientTick(const float DeltaSeconds);

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UBuildingSettings* Settings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Building")
	UPopulation* Population;

	UFUNCTION()
	void ProductionChanged(int16 Change);

	UPROPERTY(Replicated)
	ATile* Tile;

	UPROPERTY(Replicated)
	ASettlement* Settlement;

	// --------------------- base income ---------------------

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIncomeChangedSig, float, IncomeChange, EProductionType,
	                                             ProductionType);

	FOnIncomeChangedSig OnIncomeChanged;
	float GetCurrentIncomePerSecond() const;
	void AddIncomeToSettlement();

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float IncomeProgress = 0.0f;

	// ---------------- Civilian Entity ----------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	ACivilian* Civilian;

	float GetCivilianWorkRate() const;
	float GetCivilianMovementRate() const;

	UPROPERTY(VisibleInstanceOnly)
	AArmy* Army;

	UPROPERTY(VisibleInstanceOnly)
	float ArmyRespawnTimer = 0.0F;

	float GetArmyRecruitRate() const;
	float GetArmyMovementRate() const;

	//---------------------Construction phase----------------

	void SetCivilian(ACivilian* NewCivilian);

public:
	ACivilian* GetCivilian() const { return Civilian; }

	// --------------------- Construction phase ---------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bIsUnderConstruction = true;

public:
	bool GetIsUnderConstruction() const { return bIsUnderConstruction; }

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	FGameResources ResourceProgress;

public:
	FGameResources GetResourceProgress() const;
	void SetResourceProgress(const FGameResources NewResourcesProgress);

	void FinishConstruction();
};
