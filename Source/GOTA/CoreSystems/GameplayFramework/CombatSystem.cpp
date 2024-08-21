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
	for (ACombat* Combat : Combats)
	{
		if (Combat->ShouldMerge(Tile))
		{
			Combat->AddSource(Tile);
			return;
		}
	}
	ACombat* NewCombat = GetWorld()->SpawnActor<ACombat>();
	Combats.Add(NewCombat);
	NewCombat->AddSource(Tile);
}

void UCombatSystem::TriggerAllCombats()
{
	for (ACombat* Combat : Combats)
	{
		Combat->TriggerCombat();
		Combat->Destroy();
	}
	Combats.Empty();
}
