#include "Consumption.h"

#include "BuildingSettings.h"
#include "ResourceStorage.h"
#include "Engine/AssetManagerTypes.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UConsumption::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

bool UConsumption::IsSupportedForNetworking() const
{
	return true;
}

// ---------------------------------------- Lifecycle ----------------------------------------

UConsumption::UConsumption()
{
	ResourceStorage = CreateDefaultSubobject<UResourceStorage>(TEXT("ResourceStorage"));
}

void UConsumption::S_Init(UBuilding* InBuilding)
{
	Building = InBuilding;
	if (!Building.IsValid()) return;
	ResourceStorage->S_Init(Building->GetSettings()->BaseResourceLimit,
	                        true, false);
}

void UConsumption::S_Tick(float DeltaSeconds)
{
	if (ResourceStorage->GetCurrentResources() <= FGameResources::Zero())
	{
		
	}
	else
	{
		FGameResources ResourcesToRemove = FGameResources();
		ResourcesToRemove.AddConsumption(GetConsumptionPerSecond() * DeltaSeconds, GetConsumptionType());
		ResourceStorage->S_RemoveResources(ResourcesToRemove);
	}
}

void UConsumption::C_Tick(float DeltaSeconds)
{
	FGameResources ResourcesToRemove = FGameResources();
	ResourcesToRemove.AddConsumption(GetConsumptionPerSecond() * DeltaSeconds, GetConsumptionType());
	ResourceStorage->C_RemoveResources(ResourcesToRemove);
}

// ---------------------------------------- Utility ----------------------------------------

EConsumptionType UConsumption::GetConsumptionType() const
{
	if (!Building.IsValid()) return EConsumptionType::None;
	return Building->GetSettings()->ConsumptionType;
}

FGameResources UConsumption::GetCurrentResources() const
{
	return ResourceStorage->GetCurrentResources();
}

FGameResources UConsumption::GetResourceLimit() const
{
	return ResourceStorage->GetResourceLimit();
}

void UConsumption::S_AddResources(FGameResources Resources)
{
	ResourceStorage->S_AddResources(Resources);
}

bool UConsumption::ResourceStorageIsEmpty() const
{
	return ResourceStorage.
}

// --------------------------------------- Consumption ---------------------------------------

float UConsumption::GetConsumptionPerSecond() const
{
	if (!Building.IsValid() || !Building->GetSettings()->bConsumptionEnabled) return 0.0f;
	return Building->GetSettings()->BaseConsumptionPerSecond;
}
