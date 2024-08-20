#include "CombatSystem.h"

#include "GOTA/CoreSystems/Tile/Tile.h"

void UCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UCombatSystem::IsSupportedForNetworking() const
{
	return true;
}

UCombatSystem::UCombatSystem()
{
}

void UCombatSystem::RegisterCombat(ATile* Tile)
{
	CurrentCombatSources.Add(Tile);
}

void UCombatSystem::TriggerAllCombats()
{
	for (ATile* CombatSource : CurrentCombatSources)
	{
		EvaluateCombat(CombatSource);
	}
	CurrentCombatSources.Empty();
}

void UCombatSystem::EvaluateCombat(ATile* Tile)
{
	Tile->Unbuild();
	if (Tile->EnemyTileEntity)
		Tile->EnemyTileEntity->Kill();
	if (Tile->AlliedTileEntity)
		Tile->AlliedTileEntity->Kill();
}
