// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_RunnerAgent.h"

FTransform IRL_RunnerAgent::GetAgentTransform() const
{
	return FTransform();
}

ATile* IRL_RunnerAgent::GetTargetTile() const
{
	return nullptr;
}

void IRL_RunnerAgent::ResetToRandomTile()
{
}

void IRL_RunnerAgent::SetIsMoving(bool InIsMoving)
{
}

void IRL_RunnerAgent::Steer(float SteeringAngle)
{
}
