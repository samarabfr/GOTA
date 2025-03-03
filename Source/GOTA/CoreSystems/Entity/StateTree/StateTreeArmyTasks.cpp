// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeArmyTasks.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/GameplayFramework/CombatValues.h"

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

	return Army->S_TryFindPathToNearestEnemy()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyUnprotectedNormalBuilding::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Army->S_TryFindPathToNearestEnemyUnprotectedNormalBuilding()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyDefenseBuilding::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Army->S_TryFindPathToNearestEnemyDefenseBuilding()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToNearestRecruitable::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Army->S_TryFindPathToNearestRecruitable()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
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

	return Army->S_TryFindPathToGuardTile()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToNearestEnemyToGuardTile::Tick(FStateTreeExecutionContext& Context,
                                                                 const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Army->S_TryFindPathToNearestEnemyToGuardTile()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToInterceptArmy::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Army->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Army->S_TryFindPathToInterceptArmy()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}
