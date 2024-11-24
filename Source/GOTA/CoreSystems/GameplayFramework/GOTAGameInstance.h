// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GOTAGameInstance.generated.h"

UCLASS()
class GOTA_API UGOTAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	// ------------------- LifeCycle -------------------

	virtual void Init() override;
	
};
