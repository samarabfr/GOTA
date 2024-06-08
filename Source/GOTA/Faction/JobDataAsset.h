// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JobIncome.h"
#include "Engine/DataAsset.h"
#include "JobDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UJobDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Job")
	FName Name;

	UPROPERTY(EditAnywhere, Category="Job")
	TArray<FJobIncome> JobIncomeByTier;
};