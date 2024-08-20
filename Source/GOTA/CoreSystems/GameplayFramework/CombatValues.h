// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationContainer.h"
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
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Combat Values")
	int32 Attack = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Combat Values")
	int32 Defense = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Combat Values")
	int32 IndividualHP = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Combat Values")
	int32 Individuals = 0;

	int32 GetHP() const;

	void BindToPopulation(UPopulationContainer* PopCon);
	UFUNCTION()
	void UpdateIndividuals(const FPopulation ChangedBy);
};
