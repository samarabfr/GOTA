// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JobManager.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UJobManager : public UObject
{
	GENERATED_BODY()

	int32 AvailableWorkforce;
	TList<UJob> Jobs;
	
};
