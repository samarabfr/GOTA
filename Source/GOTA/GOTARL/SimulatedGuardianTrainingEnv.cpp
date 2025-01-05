// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianTrainingEnv.h"

USimulatedGuardianTrainingEnv::USimulatedGuardianTrainingEnv()
{
}

void USimulatedGuardianTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// TODO: specify rewards
}

void USimulatedGuardianTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
	const int32 AgentId)
{
	// TODO: specify early termination conditions
}

void USimulatedGuardianTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// TODO: return to starting conditions
}


