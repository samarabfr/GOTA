#include "TileAsset.h"

bool FTileAsset::IsValidFor(const FGameplayTagContainer& GameplayTagContainer)
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}

int32 FTileAsset::GetBiasAfterMultipliers(float NormalizedOceanDistance, float NormalizedRiverDistance, float NormalizedVolcanoDistance)
{
	float Bias = SpawnBias;
	if (bUseDistanceToOceanBiasMultiplier)
	{
		float Multiplier = DistanceToOceanBiasMultiplier->GetFloatValue(NormalizedOceanDistance);
		Bias *= DistanceToOceanBiasMultiplier->GetFloatValue(NormalizedOceanDistance);
	}
	if (bUseDistanceToRiverBiasMultiplier)
	{
		Bias *= DistanceToRiverBiasMultiplier->GetFloatValue(NormalizedRiverDistance);
	}
	if (bUseDistanceToVolcanoBiasMultiplier)
	{
		Bias *= DistanceToVolcanoBiasMultiplier->GetFloatValue(NormalizedVolcanoDistance);
	}
	return Bias;
}
