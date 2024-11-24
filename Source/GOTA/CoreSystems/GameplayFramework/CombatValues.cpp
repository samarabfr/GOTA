#include "CombatValues.h"
#include "Net/UnrealNetwork.h"

void UCombatValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatValues, IndividualAttack);
	DOREPLIFETIME(UCombatValues, IndividualHP);
	DOREPLIFETIME(UCombatValues, IndividualCount);
}

bool UCombatValues::IsSupportedForNetworking() const
{
	return true;
}

int32 UCombatValues::GetIndividualAttack()
{
	return IndividualAttack;
}

int32 UCombatValues::GetIndividualHP()
{
	return IndividualHP;
}

int32 UCombatValues::GetIndividualCount()
{
	return IndividualCount;
}

float UCombatValues::GetAttackSpeed()
{
	return AttackSpeed;
}

void UCombatValues::SetIndividualAttack(int32 NewAttack)
{
	IndividualAttack = NewAttack;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetIndividualHP(int32 NewIndividualHP)
{
	IndividualHP = NewIndividualHP;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetIndividualCount(int32 NewIndividuals)
{
	IndividualCount = NewIndividuals;
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
	IndividualCount = NewIndividuals;
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast(this);
}

int32 UCombatValues::GetHP() const
{
	return IndividualCount * IndividualHP;
}

int32 UCombatValues::GetAttack() const
{
	return IndividualCount * IndividualAttack;
}
