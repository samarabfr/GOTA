// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JobIncome.generated.h"

USTRUCT(BlueprintType)
struct GOTA_API FJobIncome
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Job")
	int32 Foraging = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Job")
	int32 Woodcutting = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Job")
	int32 Hunting = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Job")
	int32 Converting = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Job")
	int32 Expanding = 0;

	void SetEverythingToZero();

	FJobIncome& operator+=(const FJobIncome& Other);
};