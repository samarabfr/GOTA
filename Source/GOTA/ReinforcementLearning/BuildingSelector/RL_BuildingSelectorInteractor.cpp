// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorInteractor.h"

#include "RL_BuildingSelectorAgent.h"
#include "LearningAgentsObservations.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"

// ----------------------- LifeCycle -----------------------

URL_BuildingSelectorInteractor::URL_BuildingSelectorInteractor()
{
}

// ----------------------- Specify Observations -----------------------

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyResourceObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ResourceMap;
	ResourceMap.Add("Food", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	ResourceMap.Add("Wood", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	ResourceMap.Add("Stone", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, ResourceMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifySettlementObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> SettlementMap;
	SettlementMap.Add("Income", SpecifyResourceObservation(InObservationSchema));
	SettlementMap.Add("CurrentResources", SpecifyResourceObservation(InObservationSchema));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SettlementMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyStateObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	return SpecifySettlementObservation(InObservationSchema);
}

void URL_BuildingSelectorInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	OutObservationSchemaElement = SpecifyStateObservation(InObservationSchema);
}

// ----------------------- Make Observations -----------------------

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeResourceObservation(
	ULearningAgentsObservationObject* InObservationObject, const FGameResources Resources)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Food", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Resources.Food));
	Map.Add("Wood", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Resources.Wood));
	Map.Add("Stone", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Resources.Stone));
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeSettlementObservation(
	ULearningAgentsObservationObject* InObservationObject, const ASettlement* Settlement)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Income", MakeResourceObservation(
		        InObservationObject, Settlement ? Settlement->GetEffectivePredictedProduction() : FGameResources()));
	Map.Add("CurrentResources", MakeResourceObservation(
		        InObservationObject, Settlement ? Settlement->GetResources() : FGameResources()));
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeStateObservation(
	ULearningAgentsObservationObject* InObservationObject, IRL_BuildingSelectorAgent* Agent)
{
	return MakeSettlementObservation(InObservationObject, Agent ? Agent->GetSettlement() : nullptr);
}

void URL_BuildingSelectorInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// how the observations are gathered from the game state
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	OutObservationObjectElement = MakeStateObservation(InObservationObject, Agent);
}

void URL_BuildingSelectorInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// which actions the agents can do
	// TODO: select building
}

void URL_BuildingSelectorInteractor::PerformAgentAction_Implementation(
	const ULearningAgentsActionObject* InActionObject,
	const FLearningAgentsActionObjectElement&
	InActionObjectElement, const int32 AgentId)
{
	// how the actions are done
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
}
