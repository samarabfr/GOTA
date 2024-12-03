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

void UBuildingCivilian::S_Tick(float DeltaSeconds)
{
	UBuilding::S_Tick(DeltaSeconds);
	if (Civilian)
		Civilian->S_Tick(DeltaSeconds);
}

void UBuildingCivilian::C_Tick(const float DeltaSeconds)
{
	Super::C_Tick(DeltaSeconds);
	if (Civilian)
		Civilian->C_Tick(DeltaSeconds);
}

void UBuildingCivilian::Destroy()
{
	Super::Destroy();
	if(Civilian)
		Civilian->S_HandleDeath();
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
	ACivilian* NewCivilian = GetTile()->GetWorld()->SpawnActor<ACivilian>(GetSettings()->CivilianClass);
	NewCivilian->S_Init(this, GetTile());
	SetCivilian(NewCivilian);
}
