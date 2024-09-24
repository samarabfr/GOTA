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

UCombatSystem::UCombatSystem()
{
	static ConstructorHelpers::FObjectFinder<UGameBalanceDataAsset> DataAssetFinder(
		TEXT("/Game/CoreSystems/GameplayFramework/DA_GameBalance"));
	GameBalance = DataAssetFinder.Object;
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
	ACombat* NewCombat = Cast<ACombat>(GetWorld()->SpawnActor(GameBalance->CombatClass));
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
