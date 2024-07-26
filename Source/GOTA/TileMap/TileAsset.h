// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileAsset.generated.h"

USTRUCT(BlueprintType)
struct FTileAsset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	USkeletalMesh* SkeletalMesh= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	UAnimSequence* Animation = nullptr;
};
