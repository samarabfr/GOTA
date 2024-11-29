#include "Civilian.h"
#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void ACivilian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Building, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Settings, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, WorkAmount, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, MovementRate, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Progress, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Status, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, NetLocation, Params);
}

// ----------------------- LifeCycle -----------------------

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
	MeshComponent->SetRelativeScale3D(FVector(1, 1, 1));
	// I still don't understand why i need to set both: the ResponseChannel and CollisionEnabled
	// but this way it will only collide with ray casts, as intended
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}

void ACivilian::ServerInit(UBuilding* InBuilding, ATile* SpawnTile)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	Building = InBuilding;
	CurrentTile = SpawnTile;
	FVector NewLocation = FVector();
	SpawnTile->AddCivilian(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = Building->Settings;
	WorkAmount = BuildingSettings->CivilianProductionAmount;
	MovementRate = 100 / BuildingSettings->CivilianMoveTime;

	SetupPopSizeChanging();
}

void ACivilian::ServerTick(const float DeltaSeconds)
{
	ValidateStatus();
	ClientTick(DeltaSeconds);
	if (Progress >= 100)
	{
		if (GetStatus() == ECivilianStatus::Moving)
			Move();
		else if (GetStatus() == ECivilianStatus::Working)
			Work();
		Progress = 0.0f;
		MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
		ValidateStatus();
	}
}

void ACivilian::ClientTick(const float DeltaSeconds)
{
	if (GetStatus() == ECivilianStatus::Moving)
	{
		Progress += MovementRate * DeltaSeconds;
	}
	else if (GetStatus() == ECivilianStatus::Working)
	{
		Progress += WorkRate * DeltaSeconds / Building->GetEfficiency();
	}
}

void ACivilian::BeginDestroy()
{
	Super::BeginDestroy();
}

// -----------------------  -----------------------

void ACivilian::OnRep_Building()
{
	if (Building)
		SetupPopSizeChanging();
}

// ----------------------- Status -----------------------

void ACivilian::ValidateStatus()
{
}

void ACivilian::SetStatus(const ECivilianStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	Progress = 0.0f;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Status, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
}

// ----------------- Working ------------------------

void ACivilian::SetupPopSizeChanging()
{
	Building->GetPopulation()->OnSizeChanged.AddDynamic(this, &ACivilian::OnPopSizeChanged);
	CalculateWorkRate();
}

void ACivilian::OnPopSizeChanged(int16 Change)
{
	CalculateWorkRate();
}

void ACivilian::CalculateWorkRate()
{
	const float Pop = Building->GetPopulation()->GetSize();
	const UBuildingSettings* BuildingSettings = Building->Settings;
	float Factor = Pop / BuildingSettings->Housing;
	WorkRate = (Factor * 100) / BuildingSettings->CivilianProductionTime;
}

void ACivilian::Work()
{
}

void ACivilian::OnRep_NetLocation()
{
	SetActorLocation(NetLocation);
}

void ACivilian::SetNetLocation(const FVector& NewNetLocation)
{
	SetActorLocation(NewNetLocation);
	NetLocation = NewNetLocation;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, NetLocation, this)
}

// ----------------- Moving ------------------------

void ACivilian::Move()
{
	ATile* NewCurrent = nullptr;
	if (!Path.IsEmpty())
	{
		NewCurrent = Path.Pop();
	}
	if (!NewCurrent) return;
	CurrentTile->RemoveCivilian(this);
	FVector NewLocation = FVector();
	NewCurrent->AddCivilian(this, NewLocation);
	SetNetLocation(NewLocation);
	CurrentTile = NewCurrent;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, CurrentTile, this)
}
