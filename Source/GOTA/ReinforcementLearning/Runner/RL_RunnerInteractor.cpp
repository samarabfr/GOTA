// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_RunnerInteractor.h"

#include "RL_RunnerAgent.h"
#include "GOTA/Tile/Tile.h"

URL_RunnerInteractor::URL_RunnerInteractor()
{
}

void URL_RunnerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	OutObservationSchemaElement =
		ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, 10000.0f);
}

void URL_RunnerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// how the observations are gathered from the game state
	IRL_RunnerAgent* Agent = Cast<IRL_RunnerAgent>(GetAgent(AgentId));
	if (!Agent) return;
	if (Agent->S_GetTargetTile())
	{
		OutObservationObjectElement = ULearningAgentsObservations::MakeLocationObservation(
			InObservationObject,
			Agent->S_GetTargetTile()->GetActorTransform().GetLocation(),
			Agent->S_GetAgentTransform(),
			L"LocationObservation",
			true,
			this,
			AgentId,
			Agent->S_GetTargetTile()->GetActorTransform().GetLocation(),
			FLinearColor::Blue);
	}
	else
	{
		OutObservationObjectElement = ULearningAgentsObservations::MakeLocationObservation(
			InObservationObject,
			FVector::ZeroVector,
			Agent->S_GetAgentTransform(),
			L"LocationObservation",
			true,
			this,
			AgentId,
			FVector::ZeroVector,
			FLinearColor::Yellow);
	}
}

void URL_RunnerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// which actions the agents can do
	TMap<FName, FLearningAgentsActionSchemaElement> Elements = TMap<FName, FLearningAgentsActionSchemaElement>();
	Elements.Add(FName("Moving"), ULearningAgentsActions::SpecifyBoolAction(InActionSchema));
	Elements.Add(FName("Steering"), ULearningAgentsActions::SpecifyAngleAction(InActionSchema));
	OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, Elements);
}

void URL_RunnerInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
                                                             const FLearningAgentsActionObjectElement&
                                                             InActionObjectElement, const int32 AgentId)
{
	// how the actions are done
	IRL_RunnerAgent* Agent = Cast<IRL_RunnerAgent>(GetAgent(AgentId));
	if (!Agent) return;
	TMap<FName, FLearningAgentsActionObjectElement> ActionElements;
	ULearningAgentsActions::GetStructAction(ActionElements, InActionObject, InActionObjectElement);
	// moving
	bool bIsMoving;
	ULearningAgentsActions::GetBoolAction(bIsMoving,
	                                      InActionObject,
	                                      *ActionElements.Find(FName("Moving")),
	                                      L"BoolAction",
	                                      true,
	                                      this,
	                                      AgentId,
	                                      Agent->S_GetAgentTransform().GetLocation(),
	                                      FLinearColor::Yellow);
	Agent->S_SetIsMoving(bIsMoving);
	// steering
	float SteeringAngle;
	ULearningAgentsActions::GetAngleAction(SteeringAngle,
	                                       InActionObject,
	                                       *ActionElements.Find(FName("Steering")),
	                                       0,
	                                       L"AngleAction",
	                                       true,
	                                       this,
	                                       AgentId,
	                                       Agent->S_GetAgentTransform().GetLocation(),
	                                       Agent->S_GetAgentTransform().GetLocation(),
	                                       FLinearColor::Blue);
	Agent->S_Steer(SteeringAngle);
}
