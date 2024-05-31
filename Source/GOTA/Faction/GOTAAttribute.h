// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOTAAttribute.generated.h"

UCLASS()
class GOTA_API UGOTAAttribute : public UObject
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Delegate 
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, ChangedBy);

private:
	float OldValue;
	void OnChange();
	
public:
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	FOnAttributeChangedSignature OnChanged;
	
	// Current Value of this Attribute
	UPROPERTY(BlueprintGetter=GetCurrent, BlueprintSetter=SetCurrent, ReplicatedUsing=OnRep_Current, Category = "Attribute")
	float Current;
	
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Attribute")
	virtual float GetCurrent() const;
	
	UFUNCTION(BlueprintSetter, Category = "Attribute")
	virtual void SetCurrent(float NewValue);
	
	UFUNCTION()
	virtual void OnRep_Current();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void Add(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void Subtract(const float Subtrahend, float& Effective_Change);
	
	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void Multiply(const float Factor, float& Effective_Change);
};