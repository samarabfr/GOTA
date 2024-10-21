// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_MoveToNextTile.generated.h"

class AArmy;

USTRUCT()
struct GOTA_API FMoveToNextTileInstanceData
{
	GENERATED_BODY()

	// automatically takes from the context
	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

USTRUCT(DisplayName="Move to next Tile")
struct GOTA_API FSTT_MoveToNextTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMoveToNextTileInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
};
