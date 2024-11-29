#include "BuildingSettings.h"

float UBuildingSettings::GetDefaultPredictedProduction() const
{
	float PredictedProduction = 0.0f;
	if (bDirectProductionEnabled&& !FMath::IsNearlyZero(DirectProductionTime))
	{
		PredictedProduction += DirectProductionAmount / DirectProductionTime;
	}
	if (bCivilianEnabled && !FMath::IsNearlyZero(CivilianProductionTime))
	{
		PredictedProduction += CivilianPredictedIncomeFactor * (CivilianProductionAmount / CivilianProductionTime);
	}
	return PredictedProduction;
}

float UBuildingSettings::GetDefaultPredictedConsumption() const
{
	float PredictedConsumption = 0.0f;
	if (bDirectProductionEnabled && !FMath::IsNearlyZero(DirectProductionTime))
	{
		PredictedConsumption += DirectConsumptionAmount / DirectProductionTime;
	}
	if (bCivilianEnabled && !FMath::IsNearlyZero(CivilianProductionTime))
	{
		PredictedConsumption += CivilianPredictedIncomeFactor * (CivilianConsumptionAmount / CivilianProductionTime);
	}
	return PredictedConsumption;
}
