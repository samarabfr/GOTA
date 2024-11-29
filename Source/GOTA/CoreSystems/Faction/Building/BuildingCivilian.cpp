#include "BuildingCivilian.h"

#include "BuildingSettings.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UBuildingCivilian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UBuildingCivilian, Civilian, Params);
}

bool UBuildingCivilian::IsSupportedForNetworking() const
{
	return true;
}

// ---------------------------------------- Lifecycle ----------------------------------------

void UBuildingCivilian::ServerTick(float DeltaSeconds)
{
	UBuilding::ServerTick(DeltaSeconds);
	if (Civilian)
		Civilian->S_Tick(DeltaSeconds);
}

void UBuildingCivilian::ClientTick(const float DeltaSeconds)
{
	Super::ClientTick(DeltaSeconds);
	if (Civilian)
		Civilian->C_Tick(DeltaSeconds);
}

void UBuildingCivilian::BeginDestroy()
{
	Super::BeginDestroy();
}

// ---------------- Civilian Entity ----------------

void UBuildingCivilian::SetCivilian(ACivilian* NewCivilian)
{
	Civilian = NewCivilian;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuildingCivilian, Civilian, this)
}

void UBuildingCivilian::FinishConstruction()
{
	Super::FinishConstruction();
	ACivilian* NewCivilian = Tile->GetWorld()->SpawnActor<ACivilian>(Settings->CivilianClass);
	NewCivilian->S_Init(this, Tile);
	SetCivilian(NewCivilian);
}
