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

int32 UCombatValues::GetAttack()
{
	return Attack;
}

int32 UCombatValues::GetDefense()
{
	return Defense;
}

int32 UCombatValues::GetIndividualHP()
{
	return IndividualHP;
}

int32 UCombatValues::GetIndividuals()
{
	return Individuals;
}

void UCombatValues::SetAttack(int32 NewAttack)
{
	Attack = NewAttack;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetDefense(int32 NewDefense)
{
	Defense = NewDefense;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetIndividualHP(int32 NewIndividualHP)
{
	IndividualHP = NewIndividualHP;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetIndividuals(int32 NewIndividuals)
{
	Individuals = NewIndividuals;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetAll(int32 NewAttack, int32 NewDefense, int32 NewIndividualHP, int32 NewIndividuals)
{
	Attack = NewAttack;
	Defense = NewDefense;
	IndividualHP = NewIndividualHP;
	Individuals = NewIndividuals;
	OnChanged.Broadcast(this);
}

int32 UCombatValues::GetHP() const
{
	return Individuals * IndividualHP;
}
