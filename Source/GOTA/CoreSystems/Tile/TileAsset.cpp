#include "TileAsset.h"

bool FTileAsset::IsValidFor(const FGameplayTagContainer& GameplayTagContainer)
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}

int32 FTileAsset::GetBiasAfterMultipliers(int32 OceanDistance, int32 RiverDistance, int32 VolcanoDistance)
{
	float Bias = SpawnBias;
	if (bUseDistanceToOceanBiasMultiplier)
	{
		Bias *= DistanceToOceanBiasMultiplier->GetFloatValue(OceanDistance);
	}
	if (bUseDistanceToRiverBiasMultiplier)
	{
		Bias *= DistanceToRiverBiasMultiplier->GetFloatValue(RiverDistance);
	}
	if (bUseDistanceToVolcanoBiasMultiplier)
	{
		Bias *= DistanceToVolcanoBiasMultiplier->GetFloatValue(VolcanoDistance);
	}
	return Bias;
}
