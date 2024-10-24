#pragma once

#include "PopulationSettings.generated.h"

UCLASS()
class GOTA_API UPopulationSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 PopulationGrowthThreshold = 0;

	UPROPERTY(EditDefaultsOnly)
	float PopGrowthPerOwnPop = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float PopGrowthPerNeighborPop = 0.0f;

	bool IsStarving = false;
};

UCLASS()
class GOTA_API UPopulationSettingsDefaults : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UPopulationSettingsDefaults();
public:
	UPROPERTY(EditDefaultsOnly)
	UPopulationSettings* PopulationSettings;
};
