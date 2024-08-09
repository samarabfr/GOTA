// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/Utility/Enums.h"
#include "CoreMinimal.h"
#include "Population.generated.h"

USTRUCT(BlueprintType)
struct FPopulation : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 Size;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MaxSize;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerColonists;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian1;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian2;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian3;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian4;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MoodContent;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MoodAngry;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MoodFear;

	void SetFollower(ECultureLoyalty Culture, int32 Value);

	void SetMood(EMood Mood, int32 Value);
	
	int32 GetNativeFollowers() const;

	FPopulation operator+(const FPopulation& Other) const;

	FPopulation operator+=(const FPopulation& Other);

	FPopulation operator-(const FPopulation& Other) const;

	FPopulation operator-=(const FPopulation& Other);
};
