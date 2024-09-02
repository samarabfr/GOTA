// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guardian.h"
#include "Engine/DataAsset.h"
#include "GuardianDataAsset.generated.h"

class ACombat;

UCLASS()
class GOTA_API UGuardianDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	TSubclassOf<AGuardian> GuardianBlueprint;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Combat")
	FString Name;
};
