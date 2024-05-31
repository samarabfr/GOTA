// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTAAttribute.h"
#include "GOTAAttributeLimited.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UGOTAAttributeLimited : public UGOTAAttribute
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void SetCurrent(float NewValue) override;
	
	UPROPERTY(BlueprintGetter=GetMaximum, BlueprintSetter=SetMaximum, ReplicatedUsing=OnRep_Maximum, Category = "Attribute")
	float Maximum;

	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Attribute")
	virtual float GetMaximum() const;
	
	UFUNCTION(BlueprintSetter, Category = "Attribute")
	virtual void SetMaximum(float NewValue);
	
	UFUNCTION()
	virtual void OnRep_Maximum();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void AddMaximum(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void SubtractMaximum(const float Subtrahend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void MultiplyMaximum(const float Factor, float& Effective_Change);
};
