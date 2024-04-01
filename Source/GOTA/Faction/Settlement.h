// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/TileMap/Tile.h"
#include "SettlementAttributes.h"
#include "GameFramework/Actor.h"
#include "Settlement.generated.h"

UCLASS(Abstract, Blueprintable)
class  ASettlement : public AActor
{
	GENERATED_BODY()
	ASettlement();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//====================================================================
	//--------------------Attributes
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintGetter=GetAttributes, Replicated)
	USettlementAttributes* Attributes;

public:
	UFUNCTION(BlueprintGetter, Category="Settlement")
	USettlementAttributes* GetAttributes();

	//====================================================================
	//--------------------BorderingUnclaimedTiles
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite)
	TSet<ATile*> BorderingUnclaimedTiles;

	//====================================================================
	//--------------------ClaimedTiles
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<ATile*> ClaimedTiles;

	//====================================================================
	//--------------------ClaimColor
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	FLinearColor ClaimColor;

};