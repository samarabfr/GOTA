// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JobDataAsset.h"
#include "JobIncome.h"
#include "Job.generated.h"

UCLASS()
class GOTA_API UJob : public UObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadWrite, Category="Job")
	int32 Tier;

	UPROPERTY(BlueprintReadWrite, Category="Job")
	UJobDataAsset* DataAsset;
};