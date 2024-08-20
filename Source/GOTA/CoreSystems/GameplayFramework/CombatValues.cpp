#include "CombatValues.h"
#include "Net/UnrealNetwork.h"

void UCombatValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatValues, Attack);
	DOREPLIFETIME(UCombatValues, Defense);
	DOREPLIFETIME(UCombatValues, IndividualHP);
	DOREPLIFETIME(UCombatValues, Individuals);
}

bool UCombatValues::IsSupportedForNetworking() const
{
	return true;
}

int32 UCombatValues::GetHP() const
{
	return Individuals * IndividualHP;
}

void UCombatValues::BindToPopulation(UPopulationContainer* PopCon)
{
	PopCon->OnPopulationChanged.AddDynamic(this, &UCombatValues::UpdateIndividuals);
	UpdateIndividuals(PopCon->Population);
}

void UCombatValues::UpdateIndividuals(const FPopulation ChangedBy)
{
	Individuals += ChangedBy.Size;
}
