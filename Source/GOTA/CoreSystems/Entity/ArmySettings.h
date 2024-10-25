// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

#include "ArmySettings.generated.h"

UCLASS()
class GOTA_API UArmySettings : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ColonyArmyMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* NativeArmyMesh;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StateTreeCompletedTaskEventTag;

	UPROPERTY(EditDefaultsOnly)
	int32 GarrisonModeInterceptingRange = 4;
};
