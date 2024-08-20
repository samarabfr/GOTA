#include "Combat.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void ACombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool ACombat::DoesCombatTilesContain(ATile* Tile)
{
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if (CombatTile.Tile == Tile) return true;
	}
	return false;
}

void ACombat::AddCombatTile(FCombatTile CombatTile)
{
	CombatTiles.Add(CombatTile);
	// anfangen zu tracken
	// tile
	CombatTile.Tile->OnEntityChanged.AddDynamic(this, &ACombat::EntityChanged);
	EntityChanged(CombatTile.Tile, nullptr);
	CombatTile.Tile->OnBuildingChanged.AddDynamic(this, &ACombat::BuildingChanged);
	BuildingChanged(CombatTile.Tile);
}

void ACombat::RemoveCombatTile(FCombatTile CombatTile)
{
	CombatTiles.Remove(CombatTile);
	// aufhören zu tracken
	// tile
	CombatTile.Tile->OnEntityChanged.RemoveDynamic(this, &ACombat::EntityChanged);
	CombatTile.Tile->OnBuildingChanged.RemoveDynamic(this, &ACombat::BuildingChanged);
}

void ACombat::AddSource(ATile* Tile)
{
	if (DoesCombatTilesContain(Tile)) return;
	AddCombatTile(FCombatTile(Tile));
	for (ATile* Neighbor : Tile->Neighbors)
	{
		if (!Neighbor || DoesCombatTilesContain(Neighbor)) continue;
		AddCombatTile(FCombatTile(Neighbor));
	}
}

bool ACombat::ShouldMerge(ATile* Tile)
{
	return DoesCombatTilesContain(Tile);
}

void ACombat::EntityChanged(ATile* Tile, AEntity* OldEntity)
{
	CalcKills();
	// unregister from delegates
	if (OldEntity) OldEntity->OnCombatValuesChanged.RemoveDynamic(this, &ACombat::CombatValuesChanged);
	// register to delegates
	if (Tile->GetAlliedEntity()) Tile->GetAlliedEntity()->OnCombatValuesChanged.AddDynamic(
		this, &ACombat::CombatValuesChanged);
	if (Tile->GetEnemyEntity()) Tile->GetEnemyEntity()->OnCombatValuesChanged.AddDynamic(
		this, &ACombat::CombatValuesChanged);
}

void ACombat::BuildingChanged(ATile* Tile)
{
	CalcKills();
	// register to delegates, unregister not necessary
	if (!Tile->Building) return;
	Tile->Building->PopContainer->CombatValues->OnChanged.AddDynamic(this, &ACombat::CombatValuesChanged);
}

void ACombat::CombatValuesChanged(UCombatValues* CombatValues)
{
	CalcKills();
}

void ACombat::CalcKills()
{
	
}
