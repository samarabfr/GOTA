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
	int32 Size = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MaxSize = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerColonists = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian1 = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian2 = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian3 = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 FollowerGuardian4 = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MoodContent = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MoodAngry = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 MoodFear = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 Bows = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 Muskets = 0;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Population")
	int32 Shields = 0;

	void SetFollower(ECultureLoyalty Culture, int32 Value);

	void SetMood(EMood Mood, int32 Value);

	int32 GetFollower(ECultureLoyalty Culture) const;

	int32 GetNativeFollowers() const;

	int32 GetMood(EMood Mood) const;

	int32 SumFollower() const;
	
	int32 SumMood() const;

	bool AnyBiggerThan(const FPopulation& Other) const;
	
	FPopulation operator+(const FPopulation& Other) const;

	FPopulation operator+=(const FPopulation& Other);

	FPopulation operator-(const FPopulation& Other) const;

	FPopulation operator-() const;
	
	FPopulation operator-=(const FPopulation& Other);
};
