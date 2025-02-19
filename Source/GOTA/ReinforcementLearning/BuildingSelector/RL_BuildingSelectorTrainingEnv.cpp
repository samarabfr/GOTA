// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorTrainingEnv.h"

#include "LearningAgentsRewards.h"
#include "RL_BuildingSelectorAgent.h"

URL_BuildingSelectorTrainingEnv::URL_BuildingSelectorTrainingEnv()
{
}

void URL_BuildingSelectorTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// specifies rewards
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	float Reward = 0;
	// progress reward
	// TODO: completion Reward für gewinnen, punish für verlieren
	// TODO: heuristics: 
}

void URL_BuildingSelectorTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                 const int32 AgentId)
{
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
}

void URL_BuildingSelectorTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
}
