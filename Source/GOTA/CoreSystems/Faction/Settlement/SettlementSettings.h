// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameResources.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "SettlementSettings.generated.h"

class UBuildingSettings;

UCLASS()
class GOTA_API USettlementSettings : public UObject
{
	GENERATED_BODY()

	// ------------------- Replication Setup -------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override;

	// ------------------- Init only Settings -------------------
private:
	UPROPERTY(EditDefaultsOnly)
	EAffiliation Affiliation;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> StartingBuildings;

	UPROPERTY(EditDefaultsOnly)
	FGameResources StartingResources;

public:
	EAffiliation GetAffiliation() const { return Affiliation; }

	FGameplayTagContainer GetGameplayTags() const { return GameplayTags; }

	TArray<UBuildingSettings*> GetStartingBuildings() const { return StartingBuildings; }

	FGameResources GetStartingResources() const { return StartingResources; }

	// ------------------- Changeable Settings -------------------
private:
	UPROPERTY(EditAnywhere, Replicated)
	float PopEatingPerSecond;

	UPROPERTY(EditAnywhere, Replicated)
	float StarvingThreshold;

public:
	float GetPopEatingPerSecond() const { return PopEatingPerSecond; }
	void S_SetPopEatingPerSecond(float NewValue);

	float GetStarvingThreshold() const { return StarvingThreshold; }
	void S_SetStarvingThreshold(float NewValue);
};

// ------------------- Defaults Data Asset -------------------

UCLASS()
class GOTA_API USettlementSettingsDefaults : public UPrimaryDataAsset
{
	GENERATED_BODY()
	USettlementSettingsDefaults();

public:
	UPROPERTY(EditDefaultsOnly)
	USettlementSettings* SettlementSettings;
};
