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
	UPROPERTY(BlueprintGetter=GetAttack, BlueprintSetter=SetAttack, Replicated, Category="Combat Values")
	int32 Attack = 0;

	UPROPERTY(BlueprintGetter=GetDefense, BlueprintSetter=SetDefense, Replicated, Category="Combat Values")
	int32 Defense = 0;

	UPROPERTY(BlueprintGetter=GetIndividualHP, BlueprintSetter=SetIndividualHP, Replicated, Category="Combat Values")
	int32 IndividualHP = 0;

	UPROPERTY(BlueprintGetter=GetIndividuals, BlueprintSetter=SetIndividuals, Replicated, Category="Combat Values")
	int32 Individuals = 0;

public:
	UFUNCTION(BlueprintGetter)
	int32 GetAttack();

	UFUNCTION(BlueprintGetter)
	int32 GetDefense();

	UFUNCTION(BlueprintGetter)
	int32 GetIndividualHP();

	UFUNCTION(BlueprintGetter)
	int32 GetIndividuals();

	UFUNCTION(BlueprintSetter)
	void SetAttack(int32 NewAttack);

	UFUNCTION(BlueprintSetter)
	void SetDefense(int32 NewDefense);

	UFUNCTION(BlueprintSetter)
	void SetIndividualHP(int32 NewIndividualHP);

	UFUNCTION(BlueprintSetter)
	void SetIndividuals(int32 NewIndividuals);

	UFUNCTION(BlueprintSetter)
	void SetAll(int32 NewAttack, int32 NewDefense, int32 NewIndividualHP, int32 NewIndividuals);

	int32 GetHP() const;
};
