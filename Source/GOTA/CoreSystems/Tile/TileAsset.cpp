#include "TileAsset.h"


bool FTileAsset::IsValidFor(const FGameplayTagContainer& GameplayTagContainer) const
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}

int32 FTileAsset::GetBiasAfterMultipliers(const FTerrain& Terrain) const
{
	return SpawnBias.GetBiasAfterMultipliers(Terrain);
}
