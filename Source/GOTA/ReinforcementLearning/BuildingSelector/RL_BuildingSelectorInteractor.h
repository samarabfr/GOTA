// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsInteractor.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"

#include "RL_BuildingSelectorInteractor.generated.h"

class ASettlement;
struct FGameResources;
class IRL_BuildingSelectorAgent;

UCLASS(Blueprintable)
class GOTA_API URL_BuildingSelectorInteractor : public ULearningAgentsInteractor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	URL_BuildingSelectorInteractor();

	public:
	void Init(int32 InPossibleBuildingsCount);

	// ----------------------- Specify Observations -----------------------
private:
	static FLearningAgentsObservationSchemaElement SpecifyResourceObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	static FLearningAgentsObservationSchemaElement SpecifySettlementObservation(
		ULearningAgentsObservationSchema* InObservationSchema);
	static FLearningAgentsObservationSchemaElement SpecifyStateObservation(
		ULearningAgentsObservationSchema* InObservationSchema);

public:
	virtual void SpecifyAgentObservation_Implementation(
		FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
		ULearningAgentsObservationSchema* InObservationSchema) override;

	// ----------------------- Make Observations -----------------------
private:
	static FLearningAgentsObservationObjectElement MakeResourceObservation(
		ULearningAgentsObservationObject* InObservationObject,
		FGameResources Resources);
	static FLearningAgentsObservationObjectElement MakeSettlementObservation(
		ULearningAgentsObservationObject* InObservationObject,
		const ASettlement* Settlement);
	static FLearningAgentsObservationObjectElement MakeStateObservation(
		ULearningAgentsObservationObject* InObservationObject,
		IRL_BuildingSelectorAgent* Agent);

public:
	virtual void GatherAgentObservation_Implementation(
		FLearningAgentsObservationObjectElement& OutObservationObjectElement,
		ULearningAgentsObservationObject* InObservationObject, const int32 AgentId) override;

	// ----------------------- Actions -----------------------
	private:
	int32 PossibleBuildingsCount = 0;

public:

	virtual void SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement,
	                                               ULearningAgentsActionSchema* InActionSchema) override;

	virtual void PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
	                                               const FLearningAgentsActionObjectElement& InActionObjectElement,
	                                               const int32 AgentId) override;
};
