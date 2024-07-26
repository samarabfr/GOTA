// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileGraphicsDataAsset.generated.h"

UCLASS()
class GOTA_API UTileGraphicsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	UMaterialInstance* M_Grass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	UMaterialInstance* M_Beach;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	UMaterialInstance* M_Mountain;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Building")
	UMaterialInstance* M_Volcano;
};
