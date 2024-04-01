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
	//--------------------Nature
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetNature, BlueprintSetter=SetNature, ReplicatedUsing=OnRep_Nature)
	float Nature;

	UFUNCTION()
	void OnRep_Nature(float NewNature);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddNature(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyNature(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddNature or MultiplyNature instead"))
	void SetNature(float NewNature);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetNature();

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnNatureChanged;

	//====================================================================
	//--------------------MaxNature
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetMaxNature, BlueprintSetter=SetMaxNature, ReplicatedUsing=OnRep_MaxNature)
	float MaxNature;

	UFUNCTION()
	void OnRep_MaxNature(float NewMaxNature);

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void AddMaxNature(const float Addend, float& Effective_Change);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void MultiplyMaxNature(const float Factor, float& Effective_Change);

	UFUNCTION(BlueprintSetter, Category="Attributes",
		meta = (ToolTip = "Consider using AddMaxNature or MultiplyMaxNature instead"))
	void SetMaxNature(float NewMaxNature);

	UFUNCTION(BlueprintGetter, Category="Attributes")
	float GetMaxNature();

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributeChangedSignature OnMaxNatureChanged;
};
