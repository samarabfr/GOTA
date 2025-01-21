// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeArmyTasks.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"

EStateTreeRunStatus FSTT_AttackEnemy::EnterState(FStateTreeExecutionContext& Context,
                                                     const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.ArmyRef.Get()->S_StartAttacking();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_ArmyMoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
													const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.ArmyRef.Get()->S_StartMoveToNextTileOnPath();
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_RecruitFromTile::EnterState(FStateTreeExecutionContext& Context,
													 const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.ArmyRef.Get()->S_StartRecruitFromTile();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemy::EnterState(FStateTreeExecutionContext& Context,
													 const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestEnemy();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyUnprotectedNormalBuilding::EnterState(FStateTreeExecutionContext& Context,
													 const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestEnemyUnprotectedNormalBuilding();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyDefenseBuilding::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestEnemyDefenseBuilding();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FSTT_FindPathToNearestRecruitable::EnterState(FStateTreeExecutionContext& Context,
                                                                  const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestRecruitable();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FSTT_RavageEnemyBuilding::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.ArmyRef.Get()->S_StartRavagingEnemyBuilding();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToGuardTile::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToGuardTile();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyToGuardTile::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestEnemyToGuardTile();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FSTT_FindPathToInterceptArmy::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToInterceptArmy();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}
