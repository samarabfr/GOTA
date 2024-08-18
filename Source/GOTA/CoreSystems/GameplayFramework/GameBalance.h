// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameBalance.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UGameBalanceDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Game End")
	float GameEndingEcoThreshold;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 HumanHP = 4;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 BuildingTierHP = 20;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 ContentPopAttack = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 AngryPopAttack = 3;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 FearPopAttack = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 ContentPopDefense = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 AngryPopDefense = 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 FearPopDefense = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 MusketAttack = 5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 BowAttack = 3;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	int32 ShieldDefense = 4;
};
