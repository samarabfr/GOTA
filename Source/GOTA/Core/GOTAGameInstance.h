// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GOTAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UGOTAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameInstance")
	int32 IslandRadius;
};
