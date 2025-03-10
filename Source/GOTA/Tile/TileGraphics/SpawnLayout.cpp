#include "SpawnLayout.h"

void FSpawnLayout::AddSpawnPoint(const ETileAssetCategory Category, FSpawnPoint SpawnPoint)
{
	switch (Category)
	{
	case ETileAssetCategory::MainBuilding:
		MainBuildings.Add(SpawnPoint);
		MainBuilding = SpawnPoint;
		break;

	case ETileAssetCategory::Building:
		Buildings.Add(SpawnPoint);
		break;

	case ETileAssetCategory::Tree:
		Trees.Add(SpawnPoint);
		break;

	case ETileAssetCategory::Forage:
		Forage.Add(SpawnPoint);
		break;

	case ETileAssetCategory::Prop:
		Props.Add(SpawnPoint);
		break;

	case ETileAssetCategory::Foliage:
		Foliage.Add(SpawnPoint);
		break;

	default: ;
	}
}
