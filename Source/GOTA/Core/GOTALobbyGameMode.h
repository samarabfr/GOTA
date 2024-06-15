// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GOTALobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API AGOTALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	void Travel(FString LevelPath);
};
