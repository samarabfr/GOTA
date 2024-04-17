// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/TileMap/Tile.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class  ASettlement : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//====================================================================
	//--------------------Delegates
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyAttributeChangedSignature);

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAnyAttributeChangedSignature OnAnyAttributeChanged;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void RefreshBorderingTiles();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void CalculateTurn();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void LostClaim(const ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Settlement")
	void ClaimTile(const ATile* Tile);

	//====================================================================
	//--------------------Simple Variables
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TSet<ATile*> BorderingUnclaimedTiles;

	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Settlement")
	FLinearColor ClaimColor;
	
	//====================================================================
	//--------------------ExpectedFoodIncome
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetExpectedFoodIncome, BlueprintSetter=SetExpectedFoodIncome, ReplicatedUsing=OnRep_ExpectedFoodIncome)
	float ExpectedFoodIncome;

	UFUNCTION()
	void OnRep_ExpectedFoodIncome(float NewExpectedFoodIncome);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddExpectedFoodIncome(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void SubtractExpectedFoodIncome(const float Subtrahend, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddExpectedFoodIncome or MultiplyExpectedFoodIncome instead"))
	void SetExpectedFoodIncome(float NewExpectedFoodIncome);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetExpectedFoodIncome();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnExpectedFoodIncomeChanged;
	
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
	void SubtractPopulation(const float Subtrahend, float& Effective_Change);
	
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
	void SubtractMaxPopulation(const float Subtrahend, float& Effective_Change);
	
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
	void SubtractFood(const float Subtrahend, float& Effective_Change);
	
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
	void SubtractColonistReligion(const float Subtrahend, float& Effective_Change);
	
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
	void SubtractNativeReligion(const float Subtrahend, float& Effective_Change);
	
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