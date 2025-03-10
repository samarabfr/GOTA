#include "SpawnLayoutDataAsset.h"

USpawnLayoutDataAsset::USpawnLayoutDataAsset()
{
	Name = FName("Unnamed");
}

bool USpawnLayoutDataAsset::IsValidFor(const FGameplayTagContainer& GameplayTagContainer)
{
	for (FGameplayTagRule Rule : GameplayTagRules)
	{
		if (!Rule.IsValid(GameplayTagContainer)) return false;
	}
	return true;
}
