// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTAAttributeLimited.h"
#include "GOTA/TileMap/Tile.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class  ASettlement : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Constructor
	ASettlement();
	
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
	//--------------------Variables
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TSet<ATile*> BorderingUnclaimedTiles;

	UPROPERTY(BlueprintReadWrite, Category="Settlement")
	TArray<ATile*> ClaimedTiles;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Settlement")
	FLinearColor ClaimColor;

	//====================================================================
	//--------------------Attributes
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Food;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttribute* Wood;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttributeLimited* Population;
	
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