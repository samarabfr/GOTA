#include "SpawnBias.h"

#include "GOTA/Tile/Terrain.h"

int32 FSpawnBias::GetBiasAfterMultipliers(const FTerrain& Terrain) const
{
	float Bias = Base;
	// Terrain not setup correctly so its unusable
	if(Terrain.OceanDistance < 0) return Bias;
	if (bUseDistanceToOceanBiasMultiplier)
	{
		Bias *= DistanceToOceanBiasMultiplier->GetFloatValue(Terrain.OceanDistance);
	}
	if (bUseDistanceToRiverBiasMultiplier)
	{
		Bias *= DistanceToRiverBiasMultiplier->GetFloatValue(Terrain.RiverDistance);
	}
	if (bUseDistanceToVolcanoBiasMultiplier)
	{
		Bias *= DistanceToVolcanoBiasMultiplier->GetFloatValue(Terrain.VolcanoDistance);
	}
	return Bias;
}
