#include "Civilian.h"

#include "CivilianDataAsset.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

ACivilian::ACivilian()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; // TODO: ken bock
	PrimaryActorTick.TickInterval = 0.5f;

	ConstructorHelpers::FObjectFinder<UCivilianDataAsset> DataAsset(
		TEXT("/Game/CoreSystems/Entity/DA_Civilian"));
	CivilianDataAsset = DataAsset.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeScale3D(FVector(1, 1, 2));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACivilian::Init(ASettlement* Settlement_, ATile* SpawnTile, float WorkRate_, int32 WorkAmount_,
                     float MovementRate_)
{
	Settlement = Settlement_;
	CurrentTile = SpawnTile;
	SpawnTile->AddCivilian(this);
	WorkRate = WorkRate_;
	WorkAmount = WorkAmount_;
	MovementRate = MovementRate_;
}

void ACivilian::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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
	if(Status == NewStatus) return;
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
