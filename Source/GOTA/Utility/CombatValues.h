// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatValues.generated.h"

UCLASS(Blueprintable)
class UCombatValues : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedSig);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSig);

public:
	FOnChangedSig OnChanged;
	FOnDeathSig OnDeath;

private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Combat Values")
	int32 IndividualAttack = 1;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Combat Values")
	int32 IndividualMaxHP = 1;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Combat Values")
	int32 IndividualCount = 1;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Combat Values")
	int32 CurrentDamage = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Combat Values")
	float AttackSpeed = 1.0f;

private:
	void S_SetIndividualAttack(int32 NewAttack);
	void S_SetIndividualMaxHP(int32 NewIndividualMaxHP);
	void S_SetCurrentDamage(int32 NewCurrentDamage);
	void S_SetAttackSpeed(float NewAttackSpeed);
	
public:
	int32 GetIndividualAttack() const;
	int32 GetIndividualMaxHP() const;
	int32 GetIndividualCount() const;
	int32 GetCurrentDamage() const;
	int32 GetCurrentTotalHP() const;
	int32 GetMaxHP() const;
	int32 GetAttack() const;
	float GetAttackSpeed();
	void S_Init(int32 NewAttack, int32 NewIndividualHP, int32 NewIndividualCount, float NewAttackSpeed);
	void S_ReceiveDamage(int32 DamageAmount);
	void S_SetIndividualCount(int32 NewIndividualCount);
};
