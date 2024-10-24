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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	AGameSettings();

private:
	UPROPERTY()
	UPopulationSettings* TribePopulationSettings;
	UPROPERTY()
	UPopulationSettings* ColonyPopulationSettings;

public:
	UPopulationSettings* GetTribePopulationSettings() { return TribePopulationSettings; }
	UPopulationSettings* GetColonyPopulationSettings() { return ColonyPopulationSettings; }
};
