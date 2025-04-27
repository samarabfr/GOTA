// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsInteractor.h"

#include "RL_BuildingSelectorInteractor.generated.h"

class ASettlement;
struct FConstructionResources;
class IRL_BuildingSelectorAgent;

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
		ULearningAgentsObservationSchema* InObservationSchema, FString Name, float Scale);
	FLearningAgentsObservationSchemaElement SpecifyBuildingsObservation(
		ULearningAgentsObservationSchema* InObservationSchema, FString Name);
	FLearningAgentsObservationSchemaElement SpecifySettlementObservation(
		ULearningAgentsObservationSchema* InObservationSchema, FString Name);
	FLearningAgentsObservationSchemaElement SpecifyStateObservation(
		ULearningAgentsObservationSchema* InObservationSchema);

	UPROPERTY(EditDefaultsOnly)
	float IncomeScaling = 40.0f;
	UPROPERTY(EditDefaultsOnly)
	float ResourceStorageScaling = 1000000.0f;
	UPROPERTY(EditDefaultsOnly)
	float BuildingCountScaling = 400.0f;
	UPROPERTY(EditDefaultsOnly)
	float PopulationSizeScaling = 2000.0f;
	

public:
	virtual void SpecifyAgentObservation_Implementation(
		FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
		ULearningAgentsObservationSchema* InObservationSchema) override;

	// ----------------------- Make Observations -----------------------
private:
	FLearningAgentsObservationObjectElement MakeResourceObservation(
		ULearningAgentsObservationObject* InObservationObject,
		FConstructionResources Resources, FString Name, int32 AgentId);
	FLearningAgentsObservationObjectElement MakeBuildingsObservation(
		ULearningAgentsObservationObject* InObservationObject,
		const ASettlement* Settlement, FString Name, int32 AgentId);
	FLearningAgentsObservationObjectElement MakeSettlementObservation(
		ULearningAgentsObservationObject* InObservationObject,
		const ASettlement* Settlement, FString Name, int32 AgentId);
	FLearningAgentsObservationObjectElement MakeStateObservation(
		ULearningAgentsObservationObject* InObservationObject,
		IRL_BuildingSelectorAgent* Agent, int32 AgentId);

public:
	virtual void GatherAgentObservation_Implementation(
		FLearningAgentsObservationObjectElement& OutObservationObjectElement,
		ULearningAgentsObservationObject* InObservationObject, const int32 AgentId) override;

	// ----------------------- Actions -----------------------
private:
	int32 PossibleBuildingsCount = 9;

public:
	virtual void SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement,
	                                               ULearningAgentsActionSchema* InActionSchema) override;

	virtual void PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
	                                               const FLearningAgentsActionObjectElement& InActionObjectElement,
	                                               const int32 AgentId) override;
};
