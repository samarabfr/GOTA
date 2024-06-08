// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JobIncome.generated.h"

USTRUCT(BlueprintType)
struct GOTA_API FJobIncome
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Job")
	int32 Foraging;

	UPROPERTY(EditAnywhere, Category="Job")
	int32 Woodcutting;

	UPROPERTY(EditAnywhere, Category="Job")
	int32 Hunting;

	UPROPERTY(EditAnywhere, Category="Job")
	int32 Converting;

	UPROPERTY(EditAnywhere, Category="Job")
	int32 Expanding;
};