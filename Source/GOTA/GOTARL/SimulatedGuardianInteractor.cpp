// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianInteractor.h"

#include "GuardianSimulator.h"

USimulatedGuardianInteractor::USimulatedGuardianInteractor()
{
}

void USimulatedGuardianInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	OutObservationSchemaElement =
		ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, 10000.0f);
}

void USimulatedGuardianInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// how the observations are gathered from the game state
	AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId));
	if (GuardianSimulator)
	{
		if (GuardianSimulator->GetTargetTile())
		{
			OutObservationObjectElement = ULearningAgentsObservations::MakeLocationObservation(
				InObservationObject, GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation(),
				GuardianSimulator->GetPawn()->GetActorTransform());
		}
		else
		{
			OutObservationObjectElement = ULearningAgentsObservations::MakeLocationObservation(
			InObservationObject, FVector::ZeroVector, GuardianSimulator->GetPawn()->GetActorTransform());
		}
	}
}

void USimulatedGuardianInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// which actions the agents can do
	TMap<FName, FLearningAgentsActionSchemaElement> Elements = TMap<FName, FLearningAgentsActionSchemaElement>();
	Elements.Add(FName("Moving"), ULearningAgentsActions::SpecifyBoolAction(InActionSchema));
	Elements.Add(FName("Steering"), ULearningAgentsActions::SpecifyAngleAction(InActionSchema));
	OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, Elements);
}

void USimulatedGuardianInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
                                                                     const FLearningAgentsActionObjectElement&
                                                                     InActionObjectElement, const int32 AgentId)
{
	// how the actions are done
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		TMap<FName, FLearningAgentsActionObjectElement> ActionElements;
		ULearningAgentsActions::GetStructAction(ActionElements, InActionObject, InActionObjectElement);
		// moving
		bool bIsMoving;
		ULearningAgentsActions::GetBoolAction(bIsMoving, InActionObject, *ActionElements.Find(FName("Moving")));
		GuardianSimulator->SetIsMoving(bIsMoving);
		// steering
		float SteeringAngle;
		ULearningAgentsActions::GetAngleAction(SteeringAngle, InActionObject, *ActionElements.Find(FName("Steering")));
		FRotator Rotation = FRotator(0.f, SteeringAngle, 0.f);
		GuardianSimulator->SetMoveDirection(Rotation.Vector());
	}
}
