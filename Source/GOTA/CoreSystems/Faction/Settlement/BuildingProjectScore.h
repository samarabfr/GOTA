// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingProject.h"
#include "GameFramework/Actor.h"
#include "BuildingProjectScore.generated.h"

USTRUCT(BlueprintType)
struct FBuildingProjectScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UBuildingProject* BuildingProject = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Score = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UBuildingDataAsset* Data = nullptr; // for debugging
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 ProjectTime = 0; // for debugging
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Tier = 0; // for debugging
};
