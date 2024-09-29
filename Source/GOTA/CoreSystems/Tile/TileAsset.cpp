#include "TileAsset.h"


bool UTileAsset::IsValidFor(const FGameplayTagContainer& GameplayTagContainer) const
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}

int32 UTileAsset::GetBiasAfterMultipliers(const FTerrain& Terrain) const
{
	return SpawnBias.GetBiasAfterMultipliers(Terrain);
}

float UTileAsset::GetRotationAfterMode() const
{
	switch (RotationMode)
	{
	case ERotationMode::Random90Degree:
		return 90 * FMath::RandRange(0, 3);
	case ERotationMode::Random360Degree:
		return FMath::RandRange(0, 359);
	default:
		return 0.0f;
	}
}