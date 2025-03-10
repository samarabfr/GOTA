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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedSig, UCombatValues*, CombatValues);
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSig);

public:
	FOnChangedSig OnChanged;
	FOnDeathSig OnDeath;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetIndividualAttack, BlueprintSetter=SetIndividualAttack, Replicated,
		Category="Combat Values")
	int32 IndividualAttack = 0;

	UPROPERTY(VisibleInstanceOnly,BlueprintGetter=GetIndividualMaxHP, BlueprintSetter=SetIndividualMaxHP, Replicated, Category="Combat Values")
	int32 IndividualMaxHP = 0;

	UPROPERTY(VisibleInstanceOnly,BlueprintGetter=GetIndividualCount, BlueprintSetter=SetIndividualCount, Replicated,
		Category="Combat Values")
	int32 IndividualCount = 0;

	UPROPERTY(VisibleInstanceOnly,BlueprintGetter=GetCurrentTotalHP, BlueprintSetter=SetCurrentTotalHP, Replicated, Category="Combat Values")
	int32 CurrentTotalHP = 0;

	UPROPERTY(VisibleInstanceOnly,BlueprintGetter=GetAttackSpeed, BlueprintSetter=SetAttackSpeed, Replicated, Category="Combat Values")
	float AttackSpeed = 0.0f;

public:
	UFUNCTION(BlueprintGetter)
	int32 GetIndividualAttack();

	UFUNCTION(BlueprintGetter)
	int32 GetIndividualMaxHP();

	UFUNCTION(BlueprintGetter)
	int32 GetIndividualCount();

	UFUNCTION(BlueprintGetter)
	int32 GetCurrentTotalHP();

	UFUNCTION(BlueprintGetter)
	float GetAttackSpeed();

	UFUNCTION(BlueprintSetter)
	void SetIndividualAttack(int32 NewAttack);

	UFUNCTION(BlueprintSetter)
	void SetIndividualMaxHP(int32 NewIndividualMaxHP);

	UFUNCTION(BlueprintSetter)
	void SetIndividualCount(int32 NewIndividualCount);

	UFUNCTION(BlueprintSetter)
	void SetCurrentTotalHP(int32 NewCurrentTotalHP);

	UFUNCTION(BlueprintSetter)
	void SetAttackSpeed(float NewAttackSpeed);

	UFUNCTION(BlueprintSetter)
	void SetAll(int32 NewAttack, int32 NewIndividualHP, int32 NewIndividuals, float NewAttackSpeed);

	int32 GetMaxHP() const;
	int32 GetAttack() const;
	
};
