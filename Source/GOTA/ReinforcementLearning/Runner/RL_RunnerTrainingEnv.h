// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsTrainingEnvironment.h"

#include "RL_RunnerTrainingEnv.generated.h"

class IRL_RunnerAgent;
class URL_Runner;

UCLASS(Blueprintable)
class GOTA_API URL_RunnerTrainingEnv : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	URL_RunnerTrainingEnv();

	// ----------------------- Learning Agents plugin -----------------------
public:
	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void
	GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;

	// ----------------------- Learning  -----------------------
private:
	float LastDistance = 0.f;

	float CompletionDistance = 500.f;
	float CompletionReward = 10.0f;

	static float GetDistanceToTarget(IRL_RunnerAgent* Agent);
};
