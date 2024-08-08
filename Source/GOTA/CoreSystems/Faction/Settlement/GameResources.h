// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameResources.generated.h"

USTRUCT(BlueprintType)
struct FGameResources
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Graphics")
	int32 Food = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Graphics")
	int32 Wood = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Graphics")
	int32 Stone = 0;
};
