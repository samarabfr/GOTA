// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticMeshBatcher.h"
#include "Components/InstancedStaticMeshComponent.h"

AStaticMeshBatcher::AStaticMeshBatcher()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
}

FPrimitiveInstanceId AStaticMeshBatcher::AddStaticMeshInstance(UStaticMesh* StaticMesh, const FTransform& Transform)
{
	UInstancedStaticMeshComponent* ISMC = nullptr;
	if (!ISMC_Map.Contains(StaticMesh))
	{
		// Create ISMC for a new StaticMesh
		ISMC = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(
			UInstancedStaticMeshComponent::StaticClass(),
			false,
			FTransform::Identity,
			false));
		AddInstanceComponent(ISMC);
		ISMC->SetStaticMesh(StaticMesh);
		ISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (StaticMesh->GetMaterial(0)) ISMC->SetMaterial(0, StaticMesh->GetMaterial(0));
		ISMC_Map.Add(StaticMesh, ISMC);
	}
	else
	{
		ISMC = *ISMC_Map.Find(StaticMesh);
	}
	return ISMC->AddInstanceById(Transform);
}

void AStaticMeshBatcher::RemoveStaticMeshInstance(const UStaticMesh* StaticMesh, const FPrimitiveInstanceId& InstanceId)
{
	UInstancedStaticMeshComponent* ISMC = *ISMC_Map.Find(StaticMesh);
	ISMC->RemoveInstanceById(InstanceId);
}

void AStaticMeshBatcher::UpdateStaticMeshTransform(const UStaticMesh* StaticMesh, const FPrimitiveInstanceId& InstanceId,
                                                   const FTransform& Transform)
{
	UInstancedStaticMeshComponent* ISMC = *ISMC_Map.Find(StaticMesh);
	ISMC->UpdateInstanceTransformById(InstanceId, Transform, false, true);
}
