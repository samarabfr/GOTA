// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class ASettlement;

UCLASS()
class GOTA_API ATile : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Constructor
	ATile();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

public:
	UPROPERTY(BlueprintReadOnly)
	FHexCoords HexCoords;

	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Claim(const ASettlement* PotentialClaimant, bool& Success);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Unclaim();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Build(TSubclassOf<UBuilding> BuildingClass, bool& Success);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Unbuild();

	//====================================================================
	//--------------------Bool Flags
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite)
	bool IsWalkable;

	UPROPERTY(BlueprintReadWrite)
	bool IsClaimable;

	//====================================================================
	//--------------------Building
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
protected:
	UPROPERTY(BlueprintReadWrite)
	UBuilding* Building;
	
	//====================================================================
	//--------------------Claimant
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintSetter=SetClaimant, BlueprintGetter=GetClaimant, ReplicatedUsing=OnRep_Claimant)
	ASettlement* Claimant;

	UFUNCTION()
	void OnRep_Claimant(ASettlement* NewClaimant);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Tile")
	void ClaimantChanged();

public:
	UFUNCTION(BlueprintGetter)
	ASettlement* GetClaimant();

	UFUNCTION(BlueprintSetter)
	void SetClaimant(ASettlement* NewClaimant);

	//====================================================================
	//--------------------Trees
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetTrees, BlueprintSetter=SetTrees, ReplicatedUsing=OnRep_Trees)
	float Trees;

	UFUNCTION()
	void OnRep_Trees(float NewTrees);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddTrees(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyTrees(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddTrees or MultiplyTrees instead"))
	void SetTrees(float NewTrees);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetTrees();

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnTreesChanged;

	//====================================================================
	//--------------------MaxTrees
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetMaxTrees, BlueprintSetter=SetMaxTrees, ReplicatedUsing=OnRep_MaxTrees)
	float MaxTrees;

	UFUNCTION()
	void OnRep_MaxTrees(float NewMaxTrees);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddMaxTrees(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyMaxTrees(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddMaxTrees or MultiplyMaxTrees instead"))
	void SetMaxTrees(float NewMaxTrees);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetMaxTrees();

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnMaxTreesChanged;


//====================================================================
//--------------------Forage
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
UPROPERTY(BlueprintGetter=GetForage, BlueprintSetter=SetForage, ReplicatedUsing=OnRep_Forage)
float Forage;

UFUNCTION()
void OnRep_Forage(float NewForage);

public:
UFUNCTION(BlueprintCallable, Category="Attributes")
void AddForage(const float Addend, float& Effective_Change);

UFUNCTION(BlueprintCallable, Category="Attributes")
void MultiplyForage(const float Factor, float& Effective_Change);

UFUNCTION(BlueprintSetter, Category="Attributes",
	meta = (ToolTip = "Consider using AddForage or MultiplyForage instead"))
void SetForage(float NewForage);

UFUNCTION(BlueprintGetter, Category="Attributes")
float GetForage();

UPROPERTY(BlueprintAssignable, Category="Attributes")
FOnAttributeChangedSignature OnForageChanged;

//====================================================================
//--------------------MaxForage
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
UPROPERTY(BlueprintGetter=GetMaxForage, BlueprintSetter=SetMaxForage, ReplicatedUsing=OnRep_MaxForage)
float MaxForage;

UFUNCTION()
void OnRep_MaxForage(float NewMaxForage);

public:
UFUNCTION(BlueprintCallable, Category="Attributes")
void AddMaxForage(const float Addend, float& Effective_Change);

UFUNCTION(BlueprintCallable, Category="Attributes")
void MultiplyMaxForage(const float Factor, float& Effective_Change);

UFUNCTION(BlueprintSetter, Category="Attributes",
	meta = (ToolTip = "Consider using AddMaxForage or MultiplyMaxForage instead"))
void SetMaxForage(float NewMaxForage);

UFUNCTION(BlueprintGetter, Category="Attributes")
float GetMaxForage();

UPROPERTY(BlueprintAssignable, Category="Attributes")
FOnAttributeChangedSignature OnMaxForageChanged;

//====================================================================
//--------------------Wildlife
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
UPROPERTY(BlueprintGetter=GetWildlife, BlueprintSetter=SetWildlife, ReplicatedUsing=OnRep_Wildlife)
float Wildlife;

UFUNCTION()
void OnRep_Wildlife(float NewWildlife);

public:
UFUNCTION(BlueprintCallable, Category="Attributes")
void AddWildlife(const float Addend, float& Effective_Change);

UFUNCTION(BlueprintCallable, Category="Attributes")
void MultiplyWildlife(const float Factor, float& Effective_Change);

UFUNCTION(BlueprintSetter, Category="Attributes",
	meta = (ToolTip = "Consider using AddWildlife or MultiplyWildlife instead"))
void SetWildlife(float NewWildlife);

UFUNCTION(BlueprintGetter, Category="Attributes")
float GetWildlife();

UPROPERTY(BlueprintAssignable, Category="Attributes")
FOnAttributeChangedSignature OnWildlifeChanged;

//====================================================================
//--------------------MaxWildlife
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
UPROPERTY(BlueprintGetter=GetMaxWildlife, BlueprintSetter=SetMaxWildlife, ReplicatedUsing=OnRep_MaxWildlife)
float MaxWildlife;

UFUNCTION()
void OnRep_MaxWildlife(float NewMaxWildlife);

public:
UFUNCTION(BlueprintCallable, Category="Attributes")
void AddMaxWildlife(const float Addend, float& Effective_Change);

UFUNCTION(BlueprintCallable, Category="Attributes")
void MultiplyMaxWildlife(const float Factor, float& Effective_Change);

UFUNCTION(BlueprintSetter, Category="Attributes",
	meta = (ToolTip = "Consider using AddMaxWildlife or MultiplyMaxWildlife instead"))
void SetMaxWildlife(float NewMaxWildlife);

UFUNCTION(BlueprintGetter, Category="Attributes")
float GetMaxWildlife();

UPROPERTY(BlueprintAssignable, Category="Attributes")
FOnAttributeChangedSignature OnMaxWildlifeChanged;
};