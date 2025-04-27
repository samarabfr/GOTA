// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsTrainingEnvironment.h"

#include "RL_BuildingSelectorTrainingEnv.generated.h"

class AGS_Ingame;
class IRL_BuildingSelectorAgent;
class URL_BuildingSelector;

UCLASS(Blueprintable)
class GOTA_API URL_BuildingSelectorTrainingEnv : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
private:
	TWeakObjectPtr<AGS_Ingame> GameState;

protected:
	URL_BuildingSelectorTrainingEnv();

public:
	void Init(AGS_Ingame* InGameState);

	// ----------------------- Learning -----------------------
private:
	float RewardPerPop = 0.001;
	int16 LastPopSize = 0;

public:
	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void
	GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};
