// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorAgent.h"

#include "GOTA/Utility/Enums.h"

ASettlement* IRL_BuildingSelectorAgent::GetSettlement()
{
	return nullptr;
}

TArray<UBuildingSettings*> IRL_BuildingSelectorAgent::GetAvailableBuildings()
{
	return TArray<UBuildingSettings*>();
}

void IRL_BuildingSelectorAgent::HandleBuildingActionSelected(UBuildingSettings* Building)
{
}

EAffiliation IRL_BuildingSelectorAgent::GetAffiliation()
{
	return EAffiliation::Enemy;
}

TArray<int32> IRL_BuildingSelectorAgent::GetMilestonesReached()
{
	return TArray<int32>();
}

void IRL_BuildingSelectorAgent::IncrementMilestone(int32 MilestoneIndex)
{
}

bool IRL_BuildingSelectorAgent::CanBuild()
{
	return false;
}
