// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

UCLASS(Blueprintable)
class GOTA_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TSubclassOf<UBuilding> Building;
};
