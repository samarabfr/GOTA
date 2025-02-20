// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsInteractor.h"

#include "RL_BuildingSelectorInteractor.generated.h"

UCLASS(Blueprintable)
class GOTA_API URL_BuildingSelectorInteractor : public ULearningAgentsInteractor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	URL_BuildingSelectorInteractor();

	// ----------------------- Specify Observations -----------------------
private:
	FLearningAgentsObservationSchemaElement SpecifyResourceObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	FLearningAgentsObservationSchemaElement SpecifyDirectProductionObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	FLearningAgentsObservationSchemaElement SpecifyCivilianObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	FLearningAgentsObservationSchemaElement SpecifyArmyObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	FLearningAgentsObservationSchemaElement SpecifyDefenseObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	FLearningAgentsObservationSchemaElement SpecifyBuildingObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	FLearningAgentsObservationSchemaElement SpecifySettlementObservation(
		ULearningAgentsObservationSchema* InObservationSchema);

public:
	virtual void SpecifyAgentObservation_Implementation(
		FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
		ULearningAgentsObservationSchema* InObservationSchema) override;

	// ----------------------- Make Observations -----------------------
private:

public:
	virtual void GatherAgentObservation_Implementation(
		FLearningAgentsObservationObjectElement& OutObservationObjectElement,
		ULearningAgentsObservationObject* InObservationObject, const int32 AgentId) override;

	// ----------------------- Actions -----------------------

	virtual void SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement,
	                                               ULearningAgentsActionSchema* InActionSchema) override;

	virtual void PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
	                                               const FLearningAgentsActionObjectElement& InActionObjectElement,
	                                               const int32 AgentId) override;
};
