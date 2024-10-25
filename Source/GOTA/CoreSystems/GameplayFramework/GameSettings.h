// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationSettings.h"
#include "GameSettings.generated.h"


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
};
