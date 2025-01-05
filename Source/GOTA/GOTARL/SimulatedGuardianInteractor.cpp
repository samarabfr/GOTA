// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianInteractor.h"

USimulatedGuardianInteractor::USimulatedGuardianInteractor()
{
}

void USimulatedGuardianInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// TODO: specify which observations the agents can do
	// will be called once when initializing
}

void USimulatedGuardianInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// TODO: specify how the observations are gathered from the game state
}

void USimulatedGuardianInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// TODO: specify which actions the agents can do
	// will be called once when initializing
}

void USimulatedGuardianInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
	const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId)
{
	// TODO: specify how the actions are done
}
