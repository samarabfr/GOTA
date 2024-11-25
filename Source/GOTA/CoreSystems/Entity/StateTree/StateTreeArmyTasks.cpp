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

	InstanceData.ArmyRef.Get()->StartAttacking();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_MoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
													const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.ArmyRef.Get()->StartMoveToNextTileOnPath();
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

	InstanceData.ArmyRef.Get()->StartRecruitFromTile();

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

EStateTreeRunStatus FSTT_FindPathToNearestEnemyBuilding::EnterState(FStateTreeExecutionContext& Context,
													 const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestEnemyBuilding();
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

	InstanceData.ArmyRef.Get()->StartRavagingEnemyBuilding();

	return EStateTreeRunStatus::Running;
}
