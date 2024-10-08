#include "Civilian.h"

#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Net/UnrealNetwork.h"

void ACivilian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Building, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Settings, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

ACivilian::ACivilian()
{
	static ConstructorHelpers::FObjectFinder<UCivilianSettings> SettingsFinder(
		TEXT("/Game/CoreSystems/Entity/DA_Civilian"));
	Settings = SettingsFinder.Object;

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1, 1, 2));
	// I still don't understand why i need to set both: the ResponseChannel and CollisionEnabled
	// but this way it will only collide with ray casts, as intended
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}

void ACivilian::Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	Building = InBuilding;
	CurrentTile = SpawnTile;
	SpawnTile->AddCivilian(this);

	UBuildingSettings* BuildingSettings = Building->Settings;
	WorkRate = 100 / BuildingSettings->SecondsPerWorkCycle;
	WorkAmount = BuildingSettings->WorkAmountPerCycle;
	MovementRate = 100 / BuildingSettings->SecondsPerMove;
}

void ACivilian::GOTATick(float DeltaSeconds)
{
	ValidateStatus();
	if (GetStatus() == ECivilianStatus::Moving)
	{
		Progress += MovementRate * DeltaSeconds;
	}
	else if (GetStatus() == ECivilianStatus::Working)
	{
		Progress += WorkRate * DeltaSeconds;
	}
	if (Progress >= 100)
	{
		if (GetStatus() == ECivilianStatus::Moving)
			Move();
		else if (GetStatus() == ECivilianStatus::Working)
			Work();
		Progress = 0.0f;
		ValidateStatus();
	}
}

void ACivilian::ValidateStatus()
{
}

void ACivilian::SetStatus(ECivilianStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	Progress = 0.0f;
}

void ACivilian::Move()
{
	ATile* NewCurrent = nullptr;
	if (!Path.IsEmpty())
	{
		NewCurrent = Path.Pop();
	}
	if (!NewCurrent) return;
	CurrentTile->RemoveCivilian(this);
	NewCurrent->AddCivilian(this);
	CurrentTile = NewCurrent;
}

void ACivilian::Work()
{
}
