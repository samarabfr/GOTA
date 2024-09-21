// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewBuildingImportanceRatings.generated.h"

USTRUCT(BlueprintType)
struct FNewBuildingImportanceRatings
{
	GENERATED_BODY()

	// 0 - infinity: 0 = not important at all; the higher the value, the more important a thing is
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Food = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Wood = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ImportanceRating")
	float Stone = 0;
};
