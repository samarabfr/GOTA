#include "CombatValues.h"
#include "Net/UnrealNetwork.h"

void UCombatValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatValues, IndividualAttack);
	DOREPLIFETIME(UCombatValues, IndividualMaxHP);
	DOREPLIFETIME(UCombatValues, IndividualCount);
	DOREPLIFETIME(UCombatValues, CurrentTotalHP);
	DOREPLIFETIME(UCombatValues, AttackSpeed);
}

bool UCombatValues::IsSupportedForNetworking() const
{
	return true;
}

int32 UCombatValues::GetIndividualAttack()
{
	return IndividualAttack;
}

int32 UCombatValues::GetIndividualMaxHP()
{
	return IndividualMaxHP;
}

int32 UCombatValues::GetIndividualCount()
{
	return IndividualCount;
}

int32 UCombatValues::GetCurrentTotalHP()
{
	return CurrentTotalHP;
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

void UCombatValues::SetIndividualMaxHP(int32 NewIndividualMaxHP)
{
	int32 Difference = NewIndividualMaxHP - IndividualMaxHP;
	SetCurrentTotalHP(GetCurrentTotalHP() + Difference * IndividualCount);
	IndividualMaxHP = NewIndividualMaxHP;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetIndividualCount(int32 NewIndividualCount)
{
	int32 Difference = NewIndividualCount - IndividualCount;
	SetCurrentTotalHP(GetCurrentTotalHP() + Difference * IndividualMaxHP);
	IndividualCount = NewIndividualCount;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetCurrentTotalHP(int32 NewCurrentTotalHP)
{
	CurrentTotalHP = NewCurrentTotalHP;
	OnChanged.Broadcast(this);
	if(CurrentTotalHP < 0)
		OnDeath.Broadcast();
}

void UCombatValues::SetAttackSpeed(float NewAttackSpeed)
{
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast(this);
}

void UCombatValues::SetAll(int32 NewAttack, int32 NewIndividualHP, int32 NewIndividuals, float NewAttackSpeed)
{
	IndividualAttack = NewAttack;
	IndividualMaxHP = NewIndividualHP;
	IndividualCount = NewIndividuals;
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast(this);
}

int32 UCombatValues::GetMaxHP() const
{
	return IndividualCount * IndividualMaxHP;
}

int32 UCombatValues::GetAttack() const
{
	return IndividualCount * IndividualAttack;
}
