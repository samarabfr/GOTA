// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsTrainingEnvironment.h"

#include "RL_BuildingSelectorTrainingEnv.generated.h"

class IRL_BuildingSelectorAgent;
class URL_BuildingSelector;

UCLASS(Blueprintable)
class GOTA_API URL_BuildingSelectorTrainingEnv : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	URL_BuildingSelectorTrainingEnv();

	// ----------------------- Learning Agents plugin -----------------------
public:
	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void
	GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;

	// ----------------------- Learning  -----------------------
private:
	
};
