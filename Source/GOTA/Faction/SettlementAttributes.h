// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementAttributes.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GOTA_API USettlementAttributes : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Pull delegate 
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyAttributeChangedSignature);

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAnyAttributeChangedSignature OnAnyAttributeChanged;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

	//====================================================================
	//--------------------Population
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetPopulation, BlueprintSetter=SetPopulation, ReplicatedUsing=OnRep_Population)
	float Population;

	UFUNCTION()
	void OnRep_Population(float NewPopulation);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddPopulation(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyPopulation(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddPopulation or MultiplyPopulation instead"))
	void SetPopulation(float NewPopulation);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetPopulation();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnPopulationChanged;

	
	//====================================================================
	//--------------------MaxPopulation
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetMaxPopulation, BlueprintSetter=SetMaxPopulation, ReplicatedUsing=OnRep_MaxPopulation)
	float MaxPopulation;

	UFUNCTION()
	void OnRep_MaxPopulation(float NewMaxPopulation);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddMaxPopulation(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyMaxPopulation(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddMaxPopulation or MultiplyMaxPopulation instead"))
	void SetMaxPopulation(float NewMaxPopulation);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetMaxPopulation();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnMaxPopulationChanged;


	//====================================================================
	//--------------------Food
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetFood, BlueprintSetter=SetFood, ReplicatedUsing=OnRep_Food)
	float Food;

	UFUNCTION()
	void OnRep_Food(float NewFood);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddFood(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyFood(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddFood or MultiplyFood instead"))
	void SetFood(float NewFood);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetFood();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnFoodChanged;


	//====================================================================
	//--------------------ColonistReligion
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetColonistReligion, BlueprintSetter=SetColonistReligion,
		ReplicatedUsing=OnRep_ColonistReligion)
	float ColonistReligion;

	UFUNCTION()
	void OnRep_ColonistReligion(float NewColonistReligion);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddColonistReligion(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyColonistReligion(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddColonistReligion or MultiplyColonistReligion instead"))
	void SetColonistReligion(float NewColonistReligion);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetColonistReligion();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnColonistReligionChanged;


	//====================================================================
	//--------------------NativeReligion
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetNativeReligion, BlueprintSetter=SetNativeReligion,
		ReplicatedUsing=OnRep_NativeReligion)
	float NativeReligion;

	UFUNCTION()
	void OnRep_NativeReligion(float NewNativeReligion);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddNativeReligion(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyNativeReligion(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddNativeReligion or MultiplyNativeReligion instead"))
	void SetNativeReligion(float NewNativeReligion);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetNativeReligion();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnNativeReligionChanged;
};
