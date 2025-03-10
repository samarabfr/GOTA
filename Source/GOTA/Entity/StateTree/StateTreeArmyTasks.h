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

USTRUCT()
struct GOTA_API FArmyFindPathInstanceData
{
	GENERATED_BODY()

	// automatically takes from the context
	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;


	UPROPERTY(EditAnywhere, Category=Parameter)
	bool bOverridePathIn = true;
};

USTRUCT(DisplayName="AttackEnemy")
struct GOTA_API FSTT_AttackEnemy : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context,
	                       const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Army move to next Tile")
struct GOTA_API FSTT_ArmyMoveToNextTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context,
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
	virtual void ExitState(FStateTreeExecutionContext& Context,
	                       const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Find path to nearest enemy")
struct GOTA_API FSTT_FindPathToNearestEnemy : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find path to nearest enemy unprotected normal building")
struct GOTA_API FSTT_FindPathToNearestEnemyUnprotectedNormalBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find path to nearest enemy defense building")
struct GOTA_API FSTT_FindPathToNearestEnemyDefenseBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find Path to nearest Recruitable")
struct GOTA_API FSTT_FindPathToNearestRecruitable : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Ravage enemy building")
struct GOTA_API FSTT_RavageEnemyBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Find path to guard tile")
struct GOTA_API FSTT_FindPathToGuardTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find path to nearest enemy to guard tile")
struct GOTA_API FSTT_FindPathToNearestEnemyToGuardTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find path to intercept army")
struct GOTA_API FSTT_FindPathToInterceptArmy : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FArmyFindPathInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
