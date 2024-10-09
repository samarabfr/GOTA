// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"

#include "Building.generated.h"

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

	void SetCivilian(ACivilian* NewCivilian);

public:
	ACivilian* GetCivilian() const { return Civilian; }

	// --------------------- Construction phase ---------------------
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
