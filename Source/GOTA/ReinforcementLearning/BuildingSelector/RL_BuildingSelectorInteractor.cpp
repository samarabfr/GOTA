// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorInteractor.h"

#include "RL_BuildingSelectorAgent.h"
#include "LearningAgentsObservations.h"

URL_BuildingSelectorInteractor::URL_BuildingSelectorInteractor()
{
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyResourceObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ResourceMap;
	ResourceMap.Add("Food", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	ResourceMap.Add("Wood", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	ResourceMap.Add("Stone", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, ResourceMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyDirectProductionObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingDirectProductionMap;
	BuildingDirectProductionMap.Add("DirectProductionTime",
	                                ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDirectProductionMap.Add("DirectProductionAmount",
	                                ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDirectProductionMap.Add("DirectConsumptionAmount",
	                                ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement BuildingDirectProductionStruct =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingDirectProductionMap);
	return ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingDirectProductionStruct);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyCivilianObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingCivilianMap;
	BuildingCivilianMap.Add("CivilianProductionTime",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianProductionAmount",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianConsumptionAmount",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianInventoryLimit",
	                        SpecifyResourceObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianMoveTime",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianRespawnTime",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement BuildingCivilianStruct =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingCivilianMap);
	return ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingCivilianStruct);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyArmyObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingArmyMap;
	BuildingArmyMap.Add("SecondsPerRecruitCycle",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyMoveTime",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyRespawnTime",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyIndividualMaxHP",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyIndividualAttack",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyIndividualCount",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyAttackTime",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingArmyMap.Add("ArmyRavageTime",
	                    ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement BuildingArmyStruct =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingArmyMap);
	return ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingArmyStruct);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyDefenseObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingDefenseMap;
	BuildingDefenseMap.Add("RavageProtectionRange",
	                       ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDefenseMap.Add("DefenseIndividualMaxHP",
	                       ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDefenseMap.Add("DefenseIndividualAttack",
	                       ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDefenseMap.Add("DefenseIndividualCount",
	                       ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDefenseMap.Add("DefenseAttackTime",
	                       ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement BuildingDefenseStruct =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingDefenseMap);
	return ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingDefenseStruct);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyBuildingObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingMap;
	BuildingMap.Add("Cost", SpecifyResourceObservation(InObservationSchema));
	BuildingMap.Add("Housing", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingMap.Add("ProductionType", ULearningAgentsObservations::SpecifyEnumObservation(
		                InObservationSchema, StaticEnum<EProductionType>()));
	BuildingMap.Add("ConsumptionType", ULearningAgentsObservations::SpecifyEnumObservation(
		                InObservationSchema, StaticEnum<EConsumptionType>()));
	BuildingMap.Add("DirectProduction", SpecifyDirectProductionObservation(InObservationSchema));
	BuildingMap.Add("Civilian", SpecifyCivilianObservation(InObservationSchema));
	BuildingMap.Add("Army", SpecifyArmyObservation(InObservationSchema));
	BuildingMap.Add("Defense", SpecifyDefenseObservation(InObservationSchema));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifySettlementObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	FLearningAgentsObservationSchemaElement BuildingArrayObservation =
		ULearningAgentsObservations::SpecifyArrayObservation(InObservationSchema,
		                                                     SpecifyBuildingObservation(InObservationSchema),
		                                                     20);
	TMap<FName, FLearningAgentsObservationSchemaElement> SettlementMap;
	SettlementMap.Add("Income", SpecifyResourceObservation(InObservationSchema));
	SettlementMap.Add("CurrentResources", SpecifyResourceObservation(InObservationSchema));
	SettlementMap.Add("AvailableBuildings", BuildingArrayObservation);
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SettlementMap);
}

void URL_BuildingSelectorInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	OutObservationSchemaElement = SpecifySettlementObservation(InObservationSchema);
}

void URL_BuildingSelectorInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// how the observations are gathered from the game state
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
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
