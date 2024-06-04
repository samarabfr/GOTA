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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, int32, ChangedBy);
	
	int32 OldValue;
	virtual void OnChange();
	
public:
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	FOnAttributeChangedSignature OnChanged;
	
	// Current Value of this Attribute
	UPROPERTY(BlueprintGetter=GetCurrent, BlueprintSetter=SetCurrent, ReplicatedUsing=OnRep_Current, Category = "Attribute")
	int32 Current;
	
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Attribute")
	virtual int32 GetCurrent() const;
	
	UFUNCTION(BlueprintSetter, Category = "Attribute")
	virtual void SetCurrent(int32 NewValue);
	
	UFUNCTION()
	virtual void OnRep_Current();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void Add(const int32 Addend, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void Subtract(const int32 Subtrahend, int32& Effective_Change);
	
	UFUNCTION(BlueprintCallable, Category="Attribute")
	virtual void Multiply(const float Factor, int32& Effective_Change);
};