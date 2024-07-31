// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexCoords.generated.h"

USTRUCT(BlueprintType)
struct FHexCoords
{
	GENERATED_BODY()

public:
	static const float Gridsize;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Q;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 R;

	FHexCoords();

	FHexCoords(const int32 NewQ, const int32 NewR);
	
	bool operator==(const FHexCoords& Other) const;
	 
	bool Equals(const FHexCoords& Other) const;
	
	FHexCoords operator+(const FHexCoords& Other) const;

	friend inline uint32 GetTypeHash(const FHexCoords& Key)
	{
		uint32 HashValue = 0;
		HashValue = HashCombine(HashValue, GetTypeHash(Key.Q));
		HashValue = HashCombine(HashValue, GetTypeHash(Key.R));
		return HashValue;
	}
};