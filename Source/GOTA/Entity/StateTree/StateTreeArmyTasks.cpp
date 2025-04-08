// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeArmyTasks.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/Entity/Army.h"
#include "GOTA/Utility/CombatValues.h"

EStateTreeRunStatus FSTT_AttackEnemy::EnterState(FStateTreeExecutionContext& Context,
                                                 const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	
	TWeakObjectPtr<AArmy> WeakArmy = Army;
	Army->S_StartProgresser([WeakArmy]()
								{
									return WeakArmy.IsValid() ? WeakArmy->GetCombatValues()->GetAttackSpeed() : 0.f;
								},
								[WeakArmy]()
								{
									if (WeakArmy.IsValid()) WeakArmy->S_AttackEnemy();
								});

	return EStateTreeRunStatus::Running;
}

void FSTT_AttackEnemy::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return;

	Army->S_StopProgresser();
}

EStateTreeRunStatus FSTT_ArmyMoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
                                                        const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	
	TWeakObjectPtr<AArmy> WeakArmy = Army;
	Army->S_StartProgresser([WeakArmy]()
								{
									return WeakArmy.IsValid() ? WeakArmy->GetMovementRate() : 0.f;
								},
								[WeakArmy]()
								{
									if (WeakArmy.IsValid()) WeakArmy->S_MoveToNextTileOnPath();
								});

	return EStateTreeRunStatus::Running;
}

void FSTT_ArmyMoveToNextTile::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return;

	Army->S_StopProgresser();
}

EStateTreeRunStatus FSTT_RecruitFromTile::EnterState(FStateTreeExecutionContext& Context,
                                                     const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	
	TWeakObjectPtr<AArmy> WeakArmy = Army;
	Army->S_StartProgresser([WeakArmy]()
								{
									return WeakArmy.IsValid() ? WeakArmy->GetRecruitRate() : 0.f;
								},
								[WeakArmy]()
								{
									if (WeakArmy.IsValid()) WeakArmy->S_TakePopFromTile();
								});

	return EStateTreeRunStatus::Running;
}

void FSTT_RecruitFromTile::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return;

	Army->S_StopProgresser();
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemy::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToNearestEnemy();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToNearestEnemy::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyUnprotectedNormalBuilding::Tick(FStateTreeExecutionContext& Context,
                                                                               const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToNearestEnemyUnprotectedNormalBuilding();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToNearestEnemyUnprotectedNormalBuilding::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyDefenseBuilding::Tick(FStateTreeExecutionContext& Context,
                                                                     const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToNearestEnemyDefenseBuilding();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToNearestEnemyDefenseBuilding::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToNearestRecruitable::Tick(FStateTreeExecutionContext& Context,
                                                            const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToNearestRecruitable();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToNearestRecruitable::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_RavageEnemyBuilding::EnterState(FStateTreeExecutionContext& Context,
                                                         const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	
	TWeakObjectPtr<AArmy> WeakArmy = Army;
	Army->S_StartProgresser([WeakArmy]()
								{
									return WeakArmy.IsValid() ? WeakArmy->GetRavageSpeed() : 0.f;
								},
								[WeakArmy]()
								{
									if (WeakArmy.IsValid()) WeakArmy->S_RavageEnemyBuilding();
								});

	return EStateTreeRunStatus::Running;
}

void FSTT_RavageEnemyBuilding::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return;

	Army->S_StopProgresser();
}

EStateTreeRunStatus FSTT_FindPathToGuardTile::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToGuardTile();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToGuardTile::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyToGuardTile::Tick(FStateTreeExecutionContext& Context,
                                                                 const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToNearestEnemyToGuardTile();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToNearestEnemyToGuardTile::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToInterceptArmy::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{ 
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Army]
			{
				return Army->FindPathToInterceptArmy();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Army->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToInterceptArmy::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}
