// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerInteractor.h"

URunnerInteractor::URunnerInteractor()
{
}

void URunnerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	OutObservationSchemaElement =
		ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, 10000.0f);
}

void URunnerInteractor::GatherAgentObservation_Implementation(
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
				InObservationObject,
				GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation(),
				GuardianSimulator->GetPawn()->GetActorTransform(),
				L"LocationObservation",
				true,
				this,
				AgentId,
				GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation(),
				FLinearColor::Blue);
		}
		else
		{
			OutObservationObjectElement = ULearningAgentsObservations::MakeLocationObservation(
				InObservationObject,
				FVector::ZeroVector,
				GuardianSimulator->GetPawn()->GetActorTransform(),
				L"LocationObservation",
				true,
				this,
				AgentId,
				FVector::ZeroVector,
				FLinearColor::Yellow);
		}
	}
}

void URunnerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// which actions the agents can do
	TMap<FName, FLearningAgentsActionSchemaElement> Elements = TMap<FName, FLearningAgentsActionSchemaElement>();
	Elements.Add(FName("Moving"), ULearningAgentsActions::SpecifyBoolAction(InActionSchema));
	Elements.Add(FName("Steering"), ULearningAgentsActions::SpecifyAngleAction(InActionSchema));
	OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, Elements);
}

void URunnerInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
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
		ULearningAgentsActions::GetBoolAction(bIsMoving,
			InActionObject,
			*ActionElements.Find(FName("Moving")),
			L"BoolAction",
			true,
			this,
			AgentId,
			GuardianSimulator->GetPawn()->GetActorTransform().GetLocation(),
			FLinearColor::Yellow);
		GuardianSimulator->SetIsMoving(bIsMoving);
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
			GuardianSimulator->GetPawn()->GetActorTransform().GetLocation(),
			GuardianSimulator->GetPawn()->GetActorTransform().GetLocation(),
			FLinearColor::Blue);
		GuardianSimulator->SteerPawn(SteeringAngle);
	}
}
