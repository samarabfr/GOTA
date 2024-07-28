// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GOTA/Faction/Enums.h"
#include "BiomesDataAsset.generated.h"

UCLASS()
class GOTA_API UBiomesDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, EBiome> TagToEnum;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<EBiome, FGameplayTag> EnumToTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag AllBiomes;
};
