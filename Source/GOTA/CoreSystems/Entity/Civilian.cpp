#include "Civilian.h"

#include "CivilianDataAsset.h"

ACivilian::ACivilian()
{
	ConstructorHelpers::FObjectFinder<UCivilianDataAsset> DataAsset(
		TEXT("/Game/CoreSystems/Entity/DA_Civilian"));
	CivilianDataAsset = DataAsset.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(CivilianDataAsset->Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


