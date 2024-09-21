// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameResources.generated.h"

USTRUCT(BlueprintType)
struct FGameResources
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Resources")
	int32 Food = 0;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int32 Wood = 0;
	
	UPROPERTY(EditAnywhere, Category = "Resources")
	int32 Stone = 0;
	
	FGameResources& operator+=(const FGameResources& Addend);
	
	// Overloading > operator
	bool operator>(const FGameResources& Other) const
	{
		return Food > Other.Food && Wood > Other.Wood && Stone > Other.Stone;
	}

	// Overloading >= operator
	bool operator>=(const FGameResources& Other) const
	{
		return Food >= Other.Food && Wood >= Other.Wood && Stone >= Other.Stone;
	}
};
