#include "BuildingSettings.h"

float UBuildingSettings::GetDefaultPredictedProduction() const
{
	if (FMath::IsNearlyZero(CivilianProductionTime)) return 0.0f;
	float PredictedProduction = 0.0f;
	if (bDirectProductionEnabled)
	{
		PredictedProduction += DirectProductionAmount / DirectProductionTime;
	}
	if (bCivilianEnabled)
	{
		PredictedProduction += CivilianPredictedIncomeFactor * (CivilianProductionAmount / CivilianProductionTime);
	}
	return PredictedProduction;
}

float UBuildingSettings::GetDefaultPredictedConsumption() const
{
	if (FMath::IsNearlyZero(CivilianProductionTime)) return 0.0f;
	float PredictedConsumption = 0.0f;
	if (bDirectProductionEnabled)
	{
		PredictedConsumption += DirectConsumptionAmount / DirectProductionTime;
	}
	if (bCivilianEnabled)
	{
		PredictedConsumption += CivilianPredictedIncomeFactor * (CivilianConsumptionAmount / CivilianProductionTime);
	}
	return PredictedConsumption;
}
