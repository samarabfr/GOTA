// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorInteractor.h"

#include "RL_BuildingSelectorAgent.h"

URL_BuildingSelectorInteractor::URL_BuildingSelectorInteractor()
{
}

void URL_BuildingSelectorInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	// TODO: Incomes, current Resources
}

void URL_BuildingSelectorInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// how the observations are gathered from the game state
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	ULearningAgentsObservations::
}

void URL_BuildingSelectorInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// which actions the agents can do
	// TODO: select building
}

void URL_BuildingSelectorInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
                                                             const FLearningAgentsActionObjectElement&
                                                             InActionObjectElement, const int32 AgentId)
{
	// how the actions are done
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	
}
