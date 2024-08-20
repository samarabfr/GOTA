#include "CombatSystem.h"

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
