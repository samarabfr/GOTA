// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/GameplayFramework/PC_Ingame.h"
#include "APC_SelfPlay.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API AAPC_SelfPlay : public APC_Ingame
{
	GENERATED_BODY()

	virtual void S_Init() override;
	virtual void C_Init() override;
};
