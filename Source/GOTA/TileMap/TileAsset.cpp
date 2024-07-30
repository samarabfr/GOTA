#include "TileAsset.h"

bool FTileAsset::IsValidFor(const FGameplayTagContainer& GameplayTagContainer)
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}
