#include "CombatValues.h"
#include "Net/UnrealNetwork.h"

void UCombatValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatValues, IndividualAttack);
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
	return IndividualAttack;
}

int32 UCombatValues::GetIndividualHP()
{
	return IndividualHP;
}

int32 UCombatValues::GetIndividuals()
{
	return Individuals;
}

float UCombatValues::GetAttackSpeed()
{
	return AttackSpeed;
}

void UCombatValues::SetAttack(int32 NewAttack)
{
	IndividualAttack = NewAttack;
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

void UCombatValues::SetAttackSpeed(float NewAttackSpeed)
{
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetAll(int32 NewAttack, int32 NewIndividualHP, int32 NewIndividuals, float NewAttackSpeed)
{
	IndividualAttack = NewAttack;
	IndividualHP = NewIndividualHP;
	Individuals = NewIndividuals;
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast(this);
}

int32 UCombatValues::GetHP() const
{
	return Individuals * IndividualHP;
}

int32 UCombatValues::GetAttack() const
{
	return Individuals * IndividualAttack;
}
