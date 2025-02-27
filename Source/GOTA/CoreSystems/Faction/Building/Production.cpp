#include "Production.h"

#include "BuildingSettings.h"
#include "Engine/AssetManagerTypes.h"
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
}

bool UProduction::IsSupportedForNetworking() const
{
	return true;
}

// ---------------------------------------- Lifecycle ----------------------------------------

void UProduction::S_Init(UBuilding* InBuilding)
{
	Building = InBuilding;
	Building->OnEfficiencyChanged.AddDynamic(this, &UProduction::HandleEfficiencyChange);
	RecalculateProductionPerSecond();
}


// ---------------------------------------- Utility ----------------------------------------

EProductionType UProduction::GetProductionType() const
{
	if (!Building.IsValid()) return EProductionType::None;
	return Building->GetSettings()->ProductionType;
}

// --------------------------------------- production ---------------------------------------

void UProduction::HandleEfficiencyChange(float EfficiencyChange)
{
	RecalculateProductionPerSecond();
}

void UProduction::RecalculateProductionPerSecond()
{
	float OldProductionPerSecond = ProductionPerSecond;
	if (!Building.IsValid() || !Building->GetSettings()->bProductionEnabled)
	{
		ProductionPerSecond = 0.f;
	}
	ProductionPerSecond = Building->GetSettings()->BaseProductionPerSecond * Building->GetEfficiency();
	if (ProductionPerSecond == OldProductionPerSecond) return;
	OnProductionPerSecondChanged.Broadcast(OldProductionPerSecond - ProductionPerSecond,
										   ProductionPerSecond);
}

void UProduction::OnRep_ProductionPerSecond(float OldProductionPerSecond)
{
	OnProductionPerSecondChanged.Broadcast(OldProductionPerSecond - ProductionPerSecond,
										   ProductionPerSecond);
}

float UProduction::GetProductionPerSecond() const
{
	return ProductionPerSecond;
}
