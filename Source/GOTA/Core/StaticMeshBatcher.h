// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticMeshBatcher.generated.h"

UCLASS()
class GOTA_API AStaticMeshBatcher : public AActor
{
	GENERATED_BODY()
	AStaticMeshBatcher();
	TMap<UStaticMesh*, UInstancedStaticMeshComponent*> ISMC_Map;

public:
	FPrimitiveInstanceId AddStaticMeshInstance(UStaticMesh* StaticMesh, FTransform& Transform);
	void RemoveStaticMeshInstance(UStaticMesh* StaticMesh, FPrimitiveInstanceId& InstanceId);
	void UpdateStaticMeshTransform(UStaticMesh* StaticMesh, FPrimitiveInstanceId& InstanceId, FTransform& Transform);
};
