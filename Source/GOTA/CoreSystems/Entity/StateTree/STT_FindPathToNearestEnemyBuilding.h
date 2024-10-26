// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_FindPathToNearestEnemyBuilding.generated.h"

class AArmy;

USTRUCT()
struct GOTA_API FFindPathToNearestEnemyBuildingInstanceData
{
	GENERATED_BODY()

	// automatically takes from the context
	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

USTRUCT(DisplayName="Find path to nearest enemy building")
struct GOTA_API FSTT_FindPathToNearestEnemyBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FFindPathToNearestEnemyBuildingInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
};

