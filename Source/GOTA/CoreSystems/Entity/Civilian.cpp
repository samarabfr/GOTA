#include "Civilian.h"

#include "CivilianDataAsset.h"

ACivilian::ACivilian()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5f;
	
	ConstructorHelpers::FObjectFinder<UCivilianDataAsset> DataAsset(
		TEXT("/Game/CoreSystems/Entity/DA_Civilian"));
	CivilianDataAsset = DataAsset.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(CivilianDataAsset->Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


