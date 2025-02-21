// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticMeshBatcher.generated.h"

UCLASS()
class GOTA_API AStaticMeshBatcher : public AActor
{
	GENERATED_BODY()
	AStaticMeshBatcher();

	UPROPERTY()
	TMap<UStaticMesh*, UInstancedStaticMeshComponent*> ISMC_Map;

public:
	void Delete();
	FPrimitiveInstanceId AddStaticMeshInstance(UStaticMesh* StaticMesh, const FTransform& Transform, bool bCastShadow);
	void RemoveStaticMeshInstance(const UStaticMesh* StaticMesh, const FPrimitiveInstanceId& InstanceId);
	void UpdateStaticMeshTransform(const UStaticMesh* StaticMesh, const FPrimitiveInstanceId& InstanceId, const FTransform& Transform);
};
