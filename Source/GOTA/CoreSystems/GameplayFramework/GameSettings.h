// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSettings.generated.h"

class UColonyBrainSettings;
class UPopulationSettings;
class USettlementSettings;

UCLASS()
class GOTA_API AGameSettings : public AActor
{
	GENERATED_BODY()

	// ------------------- Replication Setup -------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddReplicatedSubObjects();

	// ------------------- LifeCycle -------------------

	AGameSettings();

	virtual void BeginPlay() override;

	// ------------------- Population Settings -------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UPopulationSettings* TribePopulationSettings;
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UPopulationSettings* ColonyPopulationSettings;

	void LoadPopulationSettings();

public:
	UPopulationSettings* GetTribePopulationSettings() { return TribePopulationSettings; }
	UPopulationSettings* GetColonyPopulationSettings() { return ColonyPopulationSettings; }

	// ------------------- Settlement Settings -------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	USettlementSettings* TribeSettings;
	UPROPERTY(VisibleInstanceOnly, Replicated)
	USettlementSettings* ColonySettings;
	UPROPERTY(VisibleInstanceOnly)
	UColonyBrainSettings* ColonyBrainSettings;

	void LoadSettlementSettings();

public:
	USettlementSettings* GetTribeSettings() { return TribeSettings; }
	USettlementSettings* GetColonySettings() { return ColonySettings; }
	UColonyBrainSettings* GetColonyBrainSettings() { return ColonyBrainSettings; }
};
