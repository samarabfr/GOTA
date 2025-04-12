// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorTrainingEnv.h"

#include "RL_BuildingSelectorAgent.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Settlement/SettlementPopulation.h"

URL_BuildingSelectorTrainingEnv::URL_BuildingSelectorTrainingEnv()
{
}

void URL_BuildingSelectorTrainingEnv::Init(AGS_Ingame* InGameState)
{
	GameState = InGameState;
}

void URL_BuildingSelectorTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// specifies rewards
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	float Reward = 0.0f;
	// completion reward
	if (GameState->GameEnded)
	{
		if (GameState->GetGameEnding() == EGameEnding::ColonistsWon
			&& Agent->GetAffiliation() == EAffiliation::Enemy ||
			GameState->GetGameEnding() == EGameEnding::NativesWon
			&& Agent->GetAffiliation() == EAffiliation::Ally)
		{
			Reward += 1.0f;
		}
		else if (GameState->GetGameEnding() != EGameEnding::SoftLocked)
		{
			Reward -= 1.0f;
		}
	}
	// progress reward from pop
	/*
	if (ASettlement* Settlement = Agent->GetSettlement())
	{
		int16 CurrentPopSize = Settlement->GetPopulation()->GetSize();
		const float PopReward = (CurrentPopSize - LastPopSize) * RewardPerPop;
		Reward += PopReward;
		LastPopSize = CurrentPopSize;
	}
	*/
	// result
	OutReward = Reward;
}

void URL_BuildingSelectorTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                           const int32 AgentId)
{
	OutCompletion = ULearningAgentsCompletions::MakeCompletionOnCondition(GameState->GameEnded);
}

void URL_BuildingSelectorTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
	LastPopSize = 0;
}
