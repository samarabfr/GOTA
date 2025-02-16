// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_RunnerAgent.h"

FTransform IRL_RunnerAgent::S_GetAgentTransform() const
{
	return FTransform();
}

ATile* IRL_RunnerAgent::S_GetTargetTile() const
{
	return nullptr;
}

void IRL_RunnerAgent::S_ResetToRandomTile()
{
}

void IRL_RunnerAgent::S_SetIsMoving(bool InIsMoving)
{
}

void IRL_RunnerAgent::S_Steer(float SteeringAngle)
{
}
