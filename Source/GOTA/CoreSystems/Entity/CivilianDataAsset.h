// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CivilianDataAsset.generated.h"

UCLASS()
class GOTA_API UCivilianDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh;
	
};
