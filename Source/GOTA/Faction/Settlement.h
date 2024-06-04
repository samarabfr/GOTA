// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTAAttributePopulation.h"
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
	UGOTAAttributePopulation* Population;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attribute")
	UGOTAAttributeLimited* Expansion;
};