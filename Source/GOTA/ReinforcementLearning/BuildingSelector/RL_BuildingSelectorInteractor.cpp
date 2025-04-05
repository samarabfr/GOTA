// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorInteractor.h"

#include "RL_BuildingSelectorAgent.h"
#include "LearningAgentsObservations.h"
#include "GOTA/Settlement/Settlement.h"

// ----------------------- LifeCycle -----------------------

URL_BuildingSelectorInteractor::URL_BuildingSelectorInteractor()
{
}

// ----------------------- Specify Observations -----------------------

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyResourceObservation(
	ULearningAgentsObservationSchema* InObservationSchema, FString Name)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ResourceMap;
	ResourceMap.Add(
		"Food", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1,
		                                                             FName(Name + "FoodResource")));
	ResourceMap.Add(
		"Wood", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1,
		                                                             FName(Name + "WoodResource")));
	ResourceMap.Add(
		"Stone", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1,
		                                                              FName(Name + "StoneResource")));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, ResourceMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyBuildingsObservation(
	ULearningAgentsObservationSchema* InObservationSchema, FString Name)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ResourceMap;
	ResourceMap.Add("CountBuildings",
	                ULearningAgentsObservations::SpecifyFloatObservation(
		                InObservationSchema, 1, FName(Name + "CountBuildings")));
	ResourceMap.Add("CountCivilianBuildings",
	                ULearningAgentsObservations::SpecifyFloatObservation(
		                InObservationSchema, 1, FName(Name + "CountCivilianBuildings")));
	ResourceMap.Add("CountArmyBuildings",
	                ULearningAgentsObservations::SpecifyFloatObservation(
		                InObservationSchema, 1, FName(Name + "CountArmyBuildings")));
	ResourceMap.Add("CountDefenseBuildings",
	                ULearningAgentsObservations::SpecifyFloatObservation(
		                InObservationSchema, 1, FName(Name + "CountDefenseBuildings")));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, ResourceMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifySettlementObservation(
	ULearningAgentsObservationSchema* InObservationSchema, FString Name)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> SettlementMap;
	SettlementMap.Add("Income", SpecifyResourceObservation(InObservationSchema, Name + "Income"));
	SettlementMap.Add("CurrentResources", SpecifyResourceObservation(InObservationSchema, Name + "CurrentResources"));
	SettlementMap.Add("Buildings", SpecifyBuildingsObservation(InObservationSchema, Name + "Buildings"));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SettlementMap);
}

FLearningAgentsObservationSchemaElement URL_BuildingSelectorInteractor::SpecifyStateObservation(
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> Map;
	Map.Add("Settlement", SpecifySettlementObservation(InObservationSchema, "Settlement"));
	Map.Add("EnemySettlement", SpecifySettlementObservation(InObservationSchema, "EnemySettlement"));
	Map.Add("CanBuild", ULearningAgentsObservations::SpecifyBoolObservation(InObservationSchema, L"CanBuild"));
	return ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, Map);
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
	ULearningAgentsObservationObject* InObservationObject, const FConstructionResources Resources, FString Name, int32
	AgentId)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Food", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Resources.Food, FName(Name + "FoodResource"), true, this, AgentId));
	Map.Add("Wood", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Resources.Wood, FName(Name + "WoodResource"), true, this, AgentId));
	Map.Add("Stone", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Resources.Stone, FName(Name + "StoneResource"), true, this, AgentId));
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeBuildingsObservation(
	ULearningAgentsObservationObject* InObservationObject, const ASettlement* Settlement, FString Name, int32 AgentId)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("CountBuildings", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Settlement ? Settlement->GetCountOfBuildings() : 0,
		        FName(Name + "CountBuildings"), true, this, AgentId));
	Map.Add("CountCivilianBuildings", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Settlement ? Settlement->GetCountOfCivilianBuildings() : 0,
		        FName(Name + "CountCivilianBuildings"), true, this, AgentId));
	Map.Add("CountArmyBuildings", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Settlement ? Settlement->GetCountOfArmyBuildings() : 0,
		        FName(Name + "CountArmyBuildings"), true, this, AgentId));
	Map.Add("CountDefenseBuildings", ULearningAgentsObservations::MakeFloatObservation(
		        InObservationObject, Settlement ? Settlement->GetCountOfDefenseBuildings() : 0,
		        FName(Name + "CountDefenseBuildings"), true, this, AgentId));
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeSettlementObservation(
	ULearningAgentsObservationObject* InObservationObject, const ASettlement* Settlement, FString Name, int32 AgentId)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Income", MakeResourceObservation(
		        InObservationObject, Settlement ? Settlement->GetEffectiveProduction() : FConstructionResources(),
		        Name + "Income", AgentId));
	Map.Add("CurrentResources", MakeResourceObservation(
		        InObservationObject, Settlement ? Settlement->GetResources() : FConstructionResources(),
		        Name + "CurrentResources", AgentId));
	Map.Add("Buildings", MakeBuildingsObservation(InObservationObject, Settlement, Name + "Buildings", AgentId));
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
}

FLearningAgentsObservationObjectElement URL_BuildingSelectorInteractor::MakeStateObservation(
	ULearningAgentsObservationObject* InObservationObject, IRL_BuildingSelectorAgent* Agent, int32 AgentId)
{
	TMap<FName, FLearningAgentsObservationObjectElement> Map;
	Map.Add("Settlement",
	        MakeSettlementObservation(InObservationObject, Agent ? Agent->GetSettlement() : nullptr, "Settlement",
	                                  AgentId));
	Map.Add("EnemySettlement",
	        MakeSettlementObservation(InObservationObject, Agent ? Agent->GetEnemySettlement() : nullptr,
	                                  "EnemySettlement", AgentId));
	Map.Add("CanBuild",
	        ULearningAgentsObservations::MakeBoolObservation(InObservationObject, Agent->CanBuild(), L"CanBuild", true,
	                                                         this, AgentId));
	return ULearningAgentsObservations::MakeStructObservation(InObservationObject, Map);
}

void URL_BuildingSelectorInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	// how the observations are gathered from the game state
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	OutObservationObjectElement = MakeStateObservation(InObservationObject, Agent, AgentId);
}

// ----------------------- Actions -----------------------

void URL_BuildingSelectorInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// which actions the agents can do
	const TArray<float> _;
	FLearningAgentsActionSchemaElement ChooseBuildingAction =
		ULearningAgentsActions::SpecifyExclusiveDiscreteAction(InActionSchema, PossibleBuildingsCount, _,
		                                                       L"ChooseBuilding");
	FLearningAgentsActionSchemaElement ChooseBuildingOptionalAction =
		ULearningAgentsActions::SpecifyOptionalAction(InActionSchema, ChooseBuildingAction, 0.5,
		                                              L"ChooseBuildingOptional");
	OutActionSchemaElement = ChooseBuildingOptionalAction;
}

void URL_BuildingSelectorInteractor::PerformAgentAction_Implementation(
	const ULearningAgentsActionObject* InActionObject,
	const FLearningAgentsActionObjectElement&
	InActionObjectElement, const int32 AgentId)
{
	// how the actions are done
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	ELearningAgentsOptionalAction ExecuteChooseBuilding;
	FLearningAgentsActionObjectElement ChooseBuildingAction;
	ULearningAgentsActions::GetOptionalAction(ExecuteChooseBuilding, ChooseBuildingAction, InActionObject,
	                                          InActionObjectElement, L"ChooseBuildingOptional");
	if (ExecuteChooseBuilding == ELearningAgentsOptionalAction::Null) return;
	int32 Index = 0;
	ULearningAgentsActions::GetExclusiveDiscreteAction(Index, InActionObject, ChooseBuildingAction,
	                                                   L"ChooseBuilding");
	TArray<UBuildingSettings*> AvailableBuildings = Agent->GetAvailableBuildings();
	if (AvailableBuildings.Num() <= Index || !Agent->CanBuild())
		return;
	Agent->HandleBuildingActionSelected(AvailableBuildings[Index]);
}
