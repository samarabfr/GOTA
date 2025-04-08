#include "CombatValues.h"
#include "Net/UnrealNetwork.h"

void UCombatValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatValues, IndividualAttack);
	DOREPLIFETIME(UCombatValues, IndividualMaxHP);
	DOREPLIFETIME(UCombatValues, IndividualCount);
	DOREPLIFETIME(UCombatValues, CurrentDamage);
	DOREPLIFETIME(UCombatValues, AttackSpeed);
}

bool UCombatValues::IsSupportedForNetworking() const
{
	return true;
}

int32 UCombatValues::GetIndividualAttack() const
{
	return IndividualAttack;
}

int32 UCombatValues::GetIndividualMaxHP() const
{
	return IndividualMaxHP;
}

int32 UCombatValues::GetIndividualCount() const
{
	return IndividualCount;
}

int32 UCombatValues::GetCurrentTotalHP() const
{
	return GetMaxHP() - GetCurrentDamage();
}

float UCombatValues::GetAttackSpeed()
{
	return AttackSpeed;
}

void UCombatValues::S_SetIndividualAttack(int32 NewAttack)
{
	if (IndividualAttack == NewAttack || NewAttack < 0)
		return;
	IndividualAttack = NewAttack;
	OnChanged.Broadcast();
}

void UCombatValues::S_SetIndividualMaxHP(int32 NewIndividualMaxHP)
{
	if (IndividualMaxHP == NewIndividualMaxHP || NewIndividualMaxHP < 0)
		return;
	IndividualMaxHP = NewIndividualMaxHP;
	OnChanged.Broadcast();
}

void UCombatValues::S_SetCurrentDamage(int32 NewCurrentDamage)
{
	if (CurrentDamage == NewCurrentDamage || NewCurrentDamage < 0)
		return;
	if (NewCurrentDamage >= GetIndividualMaxHP())
	{
		S_SetIndividualCount(GetIndividualCount() - 1);
		CurrentDamage = 0;
	}
	else
	{
		CurrentDamage = NewCurrentDamage;
	}
	OnChanged.Broadcast();
}

void UCombatValues::S_SetIndividualCount(int32 NewIndividualCount)
{
	if (IndividualCount == NewIndividualCount)
		return;
	IndividualCount = NewIndividualCount;
	OnChanged.Broadcast();
	if (IndividualCount <= 0)
	{
		OnDeath.Broadcast();
	}
}

void UCombatValues::S_SetAttackSpeed(float NewAttackSpeed)
{
	if (AttackSpeed == NewAttackSpeed || NewAttackSpeed <= 0.0f)
		return;
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast();
}

void UCombatValues::S_Init(int32 NewAttack, int32 NewIndividualHP, int32 NewIndividualCount, float NewAttackSpeed)
{
	IndividualAttack = NewAttack;
	IndividualMaxHP = NewIndividualHP;
	IndividualCount = NewIndividualCount;
	AttackSpeed = NewAttackSpeed;
	OnChanged.Broadcast();
}

void UCombatValues::S_ReceiveDamage(int32 DamageAmount)
{
	S_SetCurrentDamage(GetCurrentDamage() + DamageAmount);
}

int32 UCombatValues::GetMaxHP() const
{
	return IndividualCount * IndividualMaxHP;
}

int32 UCombatValues::GetCurrentDamage() const
{
	return CurrentDamage;
}

int32 UCombatValues::GetAttack() const
{
	return IndividualCount * IndividualAttack;
}
