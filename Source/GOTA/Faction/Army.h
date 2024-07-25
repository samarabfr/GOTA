// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEntity.h"
#include "Army.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API AArmy : public ATileEntity
{
	GENERATED_BODY()
	
	bool IsTargetValid() const;
	
public:
	virtual void CalculateMovement() override;
};
