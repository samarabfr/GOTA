// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorInteractor.h"

#include "RL_BuildingSelectorAgent.h"

URL_BuildingSelectorInteractor::URL_BuildingSelectorInteractor()
{
}

void URL_BuildingSelectorInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	// which observations the agents can do
	// Resources observation
	TMap<FName, FLearningAgentsObservationSchemaElement> ResourceMap;
	ResourceMap.Add("Food", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	ResourceMap.Add("Wood", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	ResourceMap.Add("Stone", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement ResourcesObservation =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, ResourceMap);
	// single Building: cost, Housing, ProductionType, Civilian enabled, army enabled, Defense enabled
	// if civilian, army or defense enabled then all its values also
	// Direct production
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingDirectProductionMap;
	BuildingDirectProductionMap.Add("DirectProductionTime",
	                                ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDirectProductionMap.Add("DirectProductionAmount",
	                                ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingDirectProductionMap.Add("DirectConsumptionAmount",
	                                ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement BuildingDirectProductionStruct =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingDirectProductionMap);
	FLearningAgentsObservationSchemaElement BuildingDirectProductionObservation =
		ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingDirectProductionStruct);
	// Civilian
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingCivilianMap;
	BuildingCivilianMap.Add("CivilianProductionTime",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianProductionAmount",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianConsumptionAmount",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianInventoryLimit",
	                        ResourcesObservation);
	BuildingCivilianMap.Add("CivilianMoveTime",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingCivilianMap.Add("CivilianRespawnTime",
	                        ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	FLearningAgentsObservationSchemaElement BuildingCivilianStruct =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, BuildingCivilianMap);
	FLearningAgentsObservationSchemaElement BuildingCivilianObservation =
		ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingCivilianStruct);
	// Army
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
	FLearningAgentsObservationSchemaElement BuildingArmyObservation =
		ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingArmyStruct);
	// Defense
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
	FLearningAgentsObservationSchemaElement BuildingDefenseObservation =
		ULearningAgentsObservations::SpecifyOptionalObservation(InObservationSchema, BuildingDefenseStruct);
	// Building overall
	TMap<FName, FLearningAgentsObservationSchemaElement> BuildingMap;
	BuildingMap.Add("Cost", ResourcesObservation);
	BuildingMap.Add("Housing", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema));
	BuildingMap.Add("ProductionType", ULearningAgentsObservations::SpecifyEnumObservation(
		                InObservationSchema, StaticEnum<EProductionType>()));
	BuildingMap.Add("ConsumptionType", ULearningAgentsObservations::SpecifyEnumObservation(
		                InObservationSchema, StaticEnum<EConsumptionType>()));
	BuildingMap.Add("DirectProduction", BuildingDirectProductionObservation);
	BuildingMap.Add("Civilian", BuildingCivilianObservation);
	BuildingMap.Add("Army", BuildingArmyObservation);
	BuildingMap.Add("Defense", BuildingDefenseObservation);
	FLearningAgentsObservationSchemaElement BuildingObservation = ULearningAgentsObservations::SpecifyStructObservation(
		InObservationSchema, BuildingMap);
	// All available buildings
	FLearningAgentsObservationSchemaElement BuildingArrayObservation =
		ULearningAgentsObservations::SpecifyArrayObservation(InObservationSchema, BuildingObservation, 20);
	// Settlement
	TMap<FName, FLearningAgentsObservationSchemaElement> SettlementMap;
	SettlementMap.Add("Income", ResourcesObservation);
	SettlementMap.Add("CurrentResources", ResourcesObservation);
	SettlementMap.Add("AvailableBuildings", BuildingArrayObservation);
	FLearningAgentsObservationSchemaElement SettlementObservation =
		ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SettlementMap);
	// Result
	OutObservationSchemaElement = SettlementObservation;
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
