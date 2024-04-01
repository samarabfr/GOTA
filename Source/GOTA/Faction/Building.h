// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Gota/TileMap/Tile.h"
#include "CoreMinimal.h"
#include "Building.generated.h"

class UBuildingDataAsset;

UCLASS(Blueprintable)
class GOTA_API UBuilding : public UObject
{
	GENERATED_BODY()


	//====================================================================
	//--------------------Overrideable Events
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Building")
	void OnBuild(const ATile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Building")
	void OnUnbuild(const ATile* Tile);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Building")
	void OnClaim(const ASettlement* Claimant);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Building")
	void OnUnclaim(const ASettlement* Claimant);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Building")
	void OnTurn(const ATile* Tile);
	
	//====================================================================
	//--------------------DataAsset
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UBuildingDataAsset* DataAsset;
};