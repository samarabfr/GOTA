#include "Combat.h"

#include "GOTA/CoreSystems/Tile/Tile.h"

void ACombat::AddSource(ATile* Tile)
{
	Sources.Add(Tile);
	if (!Tiles.Contains(Tile))Tiles.Add(Tile);
	for (ATile* Neighbor : Tile->Neighbors)
	{
		if (Neighbor && Tiles.Contains(Neighbor)) Tiles.Add(Neighbor);
	}
}

bool ACombat::ShouldMerge(ATile* Tile)
{
	return Tiles.Contains(Tile);
}

// tracken: Entity enters/leaves tile, Combat Values in Entity Change,   