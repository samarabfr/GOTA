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
	TMap<FName, FLearningAgentsObservationSchemaElement> SettlementMap;
	SettlementMap.Add("Income", SpecifyResourceObservation(InObservationSchema));
	SettlementMap.Add("CurrentResources", SpecifyResourceObservation(InObservationSchema));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SettlementMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyStateObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	FLearningAgentsObservationSchemaElement BuildingArrayObservation =
		ULearningAgentsObservations::SpecifyArrayObservation(InObservationSchema,
		                                                     SpecifyBuildingObservation(InObservationSchema),
		                                                     20);
	TMap<FName, FLearningAgentsObservationSchemaElement> StateMap;
	StateMap.Add("Settlement", SpecifySettlementObservation(InObservationSchema));
	StateMap.Add("AvailableBuildings", BuildingArrayObservation);
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, StateMap);
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

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeDirectProductionObservation(
	ULearningAgentsObservationObject* InObservationObject, const UBuildingSettings* Building)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("DirectProductionTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DirectProductionTime : 0.f));
	Map.Add("DirectProductionAmount",
	        ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DirectProductionAmount : 0.f));
	Map.Add("DirectConsumptionAmount",
	        ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DirectConsumptionAmount : 0.f));
	const FLearningAgentsObservationObjectElement Struct =
		ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
	const bool bIsValid = Building != nullptr && Building->bDirectProductionEnabled;
	return ULearningAgentsObservations::MakeOptionalObservation(InObservationObject, Struct,
	                                                            bIsValid
		                                                            ? ELearningAgentsOptionalObservation::Valid
		                                                            : ELearningAgentsOptionalObservation::Null);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeCivilianObservation(
	ULearningAgentsObservationObject* InObservationObject, UBuildingSettings* Building)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("CivilianProductionTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->CivilianProductionTime : 0.f));
	Map.Add("CivilianProductionAmount", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->CivilianProductionAmount : 0.f));
	Map.Add("CivilianConsumptionAmount", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->CivilianConsumptionAmount : 0.f));
	Map.Add("CivilianInventoryLimit", MakeResourceObservation(
		        InObservationObject, Building ? Building->CivilianInventoryLimit : FGameResources()));
	Map.Add("CivilianMoveTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->CivilianMoveTime : 0.f));
	Map.Add("CivilianRespawnTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->CivilianRespawnTime : 0.f));
	const FLearningAgentsObservationObjectElement Struct =
		ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
	const bool bIsValid = Building != nullptr && Building->bDirectProductionEnabled;
	return ULearningAgentsObservations::MakeOptionalObservation(InObservationObject, Struct,
	                                                            bIsValid
		                                                            ? ELearningAgentsOptionalObservation::Valid
		                                                            : ELearningAgentsOptionalObservation::Null);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeArmyObservation(
	ULearningAgentsObservationObject* InObservationObject, UBuildingSettings* Building)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("SecondsPerRecruitCycle", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->SecondsPerRecruitCycle : 0.f));
	Map.Add("ArmyMoveTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyMoveTime : 0.f));
	Map.Add("ArmyRespawnTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyRespawnTime : 0.f));
	Map.Add("ArmyIndividualMaxHP", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyIndividualMaxHP : 0.f));
	Map.Add("ArmyIndividualAttack", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyIndividualAttack : 0.f));
	Map.Add("ArmyIndividualCount", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyIndividualCount : 0.f));
	Map.Add("ArmyAttackTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyAttackTime : 0.f));
	Map.Add("ArmyRavageTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->ArmyRavageTime : 0.f));
	const FLearningAgentsObservationObjectElement Struct =
		ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
	const bool bIsValid = Building != nullptr && Building->bDirectProductionEnabled;
	return ULearningAgentsObservations::MakeOptionalObservation(InObservationObject, Struct,
	                                                            bIsValid
		                                                            ? ELearningAgentsOptionalObservation::Valid
		                                                            : ELearningAgentsOptionalObservation::Null);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeDefenseObservation(
	ULearningAgentsObservationObject* InObservationObject, UBuildingSettings* Building)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("RavageProtectionRange", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->RavageProtectionRange : 0.f));
	Map.Add("DefenseIndividualMaxHP", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DefenseIndividualMaxHP : 0.f));
	Map.Add("DefenseIndividualAttack", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DefenseIndividualAttack : 0.f));
	Map.Add("DefenseIndividualCount", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DefenseIndividualCount : 0.f));
	Map.Add("DefenseAttackTime", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->DefenseAttackTime : 0.f));
	const FLearningAgentsObservationObjectElement Struct =
		ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
	const bool bIsValid = Building != nullptr && Building->bDirectProductionEnabled;
	return ULearningAgentsObservations::MakeOptionalObservation(InObservationObject, Struct,
	                                                            bIsValid
		                                                            ? ELearningAgentsOptionalObservation::Valid
		                                                            : ELearningAgentsOptionalObservation::Null);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeBuildingObservation(
	ULearningAgentsObservationObject* InObservationObject, UBuildingSettings* Building)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Cost", MakeResourceObservation(
		        InObservationObject, Building ? Building->Cost : FGameResources()));
	Map.Add("Housing", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Building ? Building->Housing : 0.f));
	Map.Add("ProductionType", ULearningAgentsObservations::MakeEnumObservation(
		        InObservationObject, StaticEnum<EProductionType>(),
		        static_cast<uint8>(Building ? Building->ProductionType : EProductionType::None)));
	Map.Add("ConsumptionType", ULearningAgentsObservations::MakeEnumObservation(
				InObservationObject, StaticEnum<EConsumptionType>(),
				static_cast<uint8>(Building ? Building->ConsumptionType : EConsumptionType::None)));
	Map.Add("DirectProduction", MakeDirectProductionObservation(InObservationObject, Building));
	Map.Add("Civilian", MakeCivilianObservation(InObservationObject, Building));
	Map.Add("Army", MakeArmyObservation(InObservationObject, Building));
	Map.Add("Defense", MakeDefenseObservation(InObservationObject, Building));
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
	TArray<FLearningAgentsObservationObjectElement> Buildings;
	if (Agent)
	{
		for (UBuildingSettings* Building : Agent->GetAvailableBuildings())
		{
			Buildings.Add(MakeBuildingObservation(InObservationObject, Building));
		}
	}
	const FLearningAgentsObservationObjectElement BuildingArrayObservation =
		ULearningAgentsObservations::MakeArrayObservation(InObservationObject, Buildings, 20);
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Settlement", MakeSettlementObservation(
		InObservationObject, Agent ? Agent->GetSettlement() : nullptr));
	Map.Add("AvailableBuildings", BuildingArrayObservation);
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
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
