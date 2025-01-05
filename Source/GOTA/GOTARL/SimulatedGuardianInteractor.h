// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsInteractor.h"

#include "SimulatedGuardianInteractor.generated.h"

class ULearningAgentsManager;

UCLASS(Blueprintable)
class GOTA_API USimulatedGuardianInteractor : public ULearningAgentsInteractor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	USimulatedGuardianInteractor();

	// ----------------------- Learning Agents plugin -----------------------
public:
	virtual void SpecifyAgentObservation_Implementation(
		FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
		ULearningAgentsObservationSchema* InObservationSchema) override;

	virtual void GatherAgentObservation_Implementation(
		FLearningAgentsObservationObjectElement& OutObservationObjectElement,
		ULearningAgentsObservationObject* InObservationObject, const int32 AgentId) override;

	virtual void SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement,
	                                               ULearningAgentsActionSchema* InActionSchema) override;

	virtual void PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
	                                               const FLearningAgentsActionObjectElement& InActionObjectElement,
	                                               const int32 AgentId) override;
};
