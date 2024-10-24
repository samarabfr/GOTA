// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationSettings.h"
#include "GameSettings.generated.h"


class USettlementSettings;

UCLASS(Blueprintable)
class GOTA_API UGameSettings : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UGameSettings();

private:
	UPROPERTY()
	UPopulationSettings* TribePopulationSettings;
	UPROPERTY()
	UPopulationSettings* ColonyPopulationSettings;

public:
	UPopulationSettings* GetTribePopulationSettings() { return TribePopulationSettings; }
	UPopulationSettings* GetColonyPopulationSettings() { return ColonyPopulationSettings; }
};
