#pragma once

#include "CoreMinimal.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedSig);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInt32ChangedSig, int32, ChangedBy);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGrowthChangedSig,
	int32, GrowthChangedBy,
	int32, GrowthChangeChangedBy,
	int32, GrowthThresholdChangedBy);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFPopulationChangedSig, FPopulation, ChangedBy);