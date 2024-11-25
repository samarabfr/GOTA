// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeArmyTasks.generated.h"

class AArmy;

USTRUCT()
struct GOTA_API FArmyInstanceData
{
	GENERATED_BODY()

	// automatically takes from the context
	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

USTRUCT(DisplayName="AttackEnemy")
struct GOTA_API FSTT_AttackEnemy : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Move to next Tile")
struct GOTA_API FSTT_MoveToNextTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
										   const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Recruit from Tile")
struct GOTA_API FSTT_RecruitFromTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
										   const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Find path to nearest enemy")
struct GOTA_API FSTT_FindPathToNearestEnemy : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
										   const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Find path to nearest enemy building")
struct GOTA_API FSTT_FindPathToNearestEnemyBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
										   const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Find Path to nearest Recruitable")
struct GOTA_API FSTT_FindPathToNearestRecruitable : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
										   const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Ravage enemy building")
struct GOTA_API FSTT_RavageEnemyBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
										   const FStateTreeTransitionResult& Transition) const override;
};

