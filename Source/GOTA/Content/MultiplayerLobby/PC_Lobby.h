// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API APC_Lobby : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TravelClient(const FString& LevelPath);
};
