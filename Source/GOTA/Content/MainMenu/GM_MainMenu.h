// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API AGM_MainMenu : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Travel(FString LevelPath);

	UFUNCTION(BlueprintCallable)
	void TravelClient(APlayerController* PlayerController, FString LevelPath);
};
