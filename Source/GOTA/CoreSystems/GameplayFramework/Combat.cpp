#include "Combat.h"

#include "GOTA/CoreSystems/Tile/Tile.h"

void ACombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool ACombat::DoesCombatTilesContain(ATile* Tile)
{
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if(CombatTile.Tile == Tile) return true;
	}
	return false;
}

void ACombat::AddCombatTile(FCombatTile CombatTile)
{
	CombatTiles.Add(CombatTile);
	// anfangen zu tracken
	// tile
}

void ACombat::RemoveCombatTile(FCombatTile CombatTile)
{
	CombatTiles.Remove(CombatTile);
	// aufhören zu tracken
	
}

void ACombat::AddSource(ATile* Tile)
{
	if(DoesCombatTilesContain(Tile)) return;
	AddCombatTile(FCombatTile(Tile));
	for (ATile* Neighbor : Tile->Neighbors)
	{
		if(!Neighbor || DoesCombatTilesContain(Neighbor)) continue;
		AddCombatTile(FCombatTile(Neighbor));
	}
}

bool ACombat::ShouldMerge(ATile* Tile)
{
	return DoesCombatTilesContain(Tile);
}

// tracken: Entity enters/leaves tile, Combat Values in Entity Change, Combat Values in buildings change
