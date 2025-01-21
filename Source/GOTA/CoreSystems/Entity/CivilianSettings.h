// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

#include "CivilianSettings.generated.h"

UCLASS()
class GOTA_API UCivilianSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* WoodCutterMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* BuilderMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ForagerMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* HunterMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MigrantMesh;
};
