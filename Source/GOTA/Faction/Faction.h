// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Settlement.h"
#include "GameFramework/Actor.h"
#include "Faction.generated.h"

UCLASS(Abstract, Blueprintable)
class AFaction : public AActor
{
	GENERATED_BODY()
	
	//====================================================================
	//--------------------Simple Variables
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadOnly, Category="Faction")
	TArray<ASettlement*> Settlements;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Faction")
	TSubclassOf<ASettlement> SettlementBlueprint;
	
	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Faction")
	void CreateSettlement(ASettlement*& Settlement);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Faction")
	void Init(int32 StartingSettlementCount);
	
};