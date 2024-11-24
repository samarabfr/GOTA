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

public:
	FOnChangedSig OnChanged;

private:
	UPROPERTY(BlueprintGetter=GetIndividualAttack, BlueprintSetter=SetIndividualAttack, Replicated,
		Category="Combat Values")
	int32 IndividualAttack = 0;

	UPROPERTY(BlueprintGetter=GetIndividualHP, BlueprintSetter=SetIndividualHP, Replicated, Category="Combat Values")
	int32 IndividualHP = 0;

	UPROPERTY(BlueprintGetter=GetIndividualCount, BlueprintSetter=SetIndividualCount, Replicated,
		Category="Combat Values")
	int32 IndividualCount = 0;

	UPROPERTY(BlueprintGetter=GetAttackSpeed, BlueprintSetter=SetAttackSpeed, Replicated, Category="Combat Values")
	float AttackSpeed = 0.0f;

public:
	UFUNCTION(BlueprintGetter)
	int32 GetIndividualAttack();

	UFUNCTION(BlueprintGetter)
	int32 GetIndividualHP();

	UFUNCTION(BlueprintGetter)
	int32 GetIndividualCount();

	UFUNCTION(BlueprintGetter)
	float GetAttackSpeed();

	UFUNCTION(BlueprintSetter)
	void SetIndividualAttack(int32 NewAttack);

	UFUNCTION(BlueprintSetter)
	void SetIndividualHP(int32 NewIndividualHP);

	UFUNCTION(BlueprintSetter)
	void SetIndividualCount(int32 NewIndividuals);

	UFUNCTION(BlueprintSetter)
	void SetAttackSpeed(float NewAttackSpeed);

	UFUNCTION(BlueprintSetter)
	void SetAll(int32 NewAttack, int32 NewIndividualHP, int32 NewIndividuals, float NewAttackSpeed);

	int32 GetHP() const;

	int32 GetAttack() const;
};
