#include "Combat.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
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
	CalcAttackDefense();
	int32 AlliedDamage = AlliedAttack - EnemyDefense;
	int32 EnemyDamage = EnemyAttack - AlliedDefense;
	SpreadDamage(AlliedDamage, EAffiliation::Ally);
	SpreadDamage(EnemyDamage, EAffiliation::Enemy);
}

void ACombat::CalcAttackDefense()
{
	AlliedAttack = 0;
	AlliedDefense = 0;
	EnemyAttack = 0;
	EnemyDefense = 0;
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if(CombatTile.Tile->GetAlliedEntity())
		{
			AlliedAttack += CombatTile.Tile->GetAlliedEntity()->GetAttack();
			AlliedDefense += CombatTile.Tile->GetAlliedEntity()->GetDefense();
		}
		if(CombatTile.Tile->GetEnemyEntity())
		{
			EnemyAttack += CombatTile.Tile->GetEnemyEntity()->GetAttack();
			EnemyDefense += CombatTile.Tile->GetEnemyEntity()->GetDefense();
		}
		if(CombatTile.Tile->Building
			&& CombatTile.Tile->GetClaimant())
		{
			UCombatValues* CV = CombatTile.Tile->Building->GetCombatValues();
			if(CombatTile.Tile->GetClaimant()->Affiliation == EAffiliation::Ally)
			{
				AlliedAttack += CV->GetAttack();
				AlliedDefense += CV->GetDefense();
			}
			else
			{
				EnemyAttack += CV->GetAttack();
				EnemyDefense += CV->GetDefense();
			}
		}
	}
}

void ACombat::SpreadDamage(int32 Damage, EAffiliation Affiliation)
{
	
}
