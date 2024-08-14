// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SettlementImportanceRatings.generated.h"

USTRUCT(BlueprintType)
struct FSettlementImportanceRatings
{
	GENERATED_BODY()

	// 0 - infinity: 0 = not important at all; the higher the value, the more important a thing is
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Food = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Wood = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Stone = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Bows = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Muskets = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Shields = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Housing = 0;
};
