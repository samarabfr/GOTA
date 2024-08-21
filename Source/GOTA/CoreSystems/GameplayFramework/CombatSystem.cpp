#include "CombatSystem.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void UCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UCombatSystem::IsSupportedForNetworking() const
{
	return true;
}

void UCombatSystem::RegisterCombat(ATile* Tile)
{
	
	Combats.Add(Tile);
}

void UCombatSystem::TriggerAllCombats()
{
	for (ATile* CombatSource : Combats)
	{
		EvaluateCombat(CombatSource);
	}
	Combats.Empty();
}
