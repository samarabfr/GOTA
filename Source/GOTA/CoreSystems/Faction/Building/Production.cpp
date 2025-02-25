#include "Production.h"

#include "BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UProduction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UProduction, ProductionProgress, Params);
}

bool UProduction::IsSupportedForNetworking() const
{
	return true;
}

void UProduction::S_Init(UBuilding* InBuilding)
{
	Building = InBuilding;
	Building->OnEfficiencyChanged.AddDynamic(this, &UProduction::HandleEfficiencyChange);
}

// ---------------------------------------- Lifecycle ----------------------------------------

void UProduction::S_Tick(float DeltaSeconds)
{
	if (ProductionProgress < GetEffectiveProductionTime())
	{
		ProductionProgress += DeltaSeconds;
	}
	else
	{
		S_ApplyProduction();
		ProductionProgress = 0.0f;
		MARK_PROPERTY_DIRTY_FROM_NAME(UProduction, ProductionProgress, this)
	}
}

void UProduction::C_Tick(const float DeltaSeconds)
{
	ProductionProgress += DeltaSeconds;
}

// ---------------------------------------- Utility ----------------------------------------

EProductionType UProduction::GetProductionType() const
{
	if (!Building.IsValid()) return EProductionType::None;
	return Building->GetSettings()->ProductionType;
}

// --------------------------------------- Effective production ---------------------------------------

void UProduction::HandleEfficiencyChange(float EfficiencyChange)
{
	OnEffectiveProductionChanged.Broadcast();
}

float UProduction::GetEffectiveProductionPerSecond() const
{
	if (!Building.IsValid()) return 0.0f;
	return Building->GetSettings()->ProductionAmount / GetEffectiveProductionTime();
}

float UProduction::GetEffectiveProductionTime() const
{
	if (!Building.IsValid()) return 0.0f;
	return Building->GetSettings()->ProductionTime / Building->GetEfficiency();
}

// ---------------------------------------- Progress ----------------------------------------

void UProduction::S_ApplyProduction()
{
	if (!Building.IsValid() || !Building->GetSettlement()) return;
	FGameResources NewResources;
	NewResources.AddProduction(Building->GetSettings()->ProductionAmount, Building->GetSettings()->ProductionType);
	Building->GetSettlement()->S_AddResources(NewResources);
}

float UProduction::GetProductionProgress() const
{
	return ProductionProgress;
}

