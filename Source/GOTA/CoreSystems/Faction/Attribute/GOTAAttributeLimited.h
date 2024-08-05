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
	// Unreal Engine Mystery Code
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetMaximum, BlueprintSetter=SetMaximum, ReplicatedUsing=OnRep_Maximum, Category = "Attribute")
	int32 Maximum;
	
public:
	virtual void SetCurrent(int32 NewValue) override;
	
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Attribute")
	virtual int32 GetMaximum() const;
	
	UFUNCTION(BlueprintSetter, Category = "Attribute")
	virtual void SetMaximum(int32 NewValue);
	
	UFUNCTION()
	virtual void OnRep_Maximum();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void AddMaximum(const int32 Addend, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void SubtractMaximum(const int32 Subtrahend, int32& Effective_Change);
};
