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

public:
	// Default 15 for singleplayer, kinda wierd right now but it is what it is
	UPROPERTY(BlueprintReadWrite, Category="GOTAGameInstance")
	int32 IslandRadius = 75;

	// Default 1 because of singleplayer. Lobby overrides the 1 with the correct playercount if multiplayer
	UPROPERTY(BlueprintReadWrite, Category="GOTAGameInstance")
	int32 PlayerCount = 1;
	
	// Default 4 for of singleplayer.
	UPROPERTY(BlueprintReadWrite, Category="GOTAGameInstance")
	int32 NativesSettlementCount = 4;

	// Default 8 for of singleplayer.
	UPROPERTY(BlueprintReadWrite, Category="GOTAGameInstance")
	int32 ColonistsSettlementCount = 8;
	
	// Lobby players
	UPROPERTY(BlueprintReadWrite, Category="GOTALobbyGameState")
	TSubclassOf<class AGuardian> SelectedGuardian1;

	UPROPERTY(BlueprintReadWrite, Category="GOTALobbyGameState")
	TSubclassOf<class AGuardian> SelectedGuardian2;

	UPROPERTY(BlueprintReadWrite, Category="GOTALobbyGameState")
	TSubclassOf<class AGuardian> SelectedGuardian3;

	UPROPERTY(BlueprintReadWrite, Category="GOTALobbyGameState")
	TSubclassOf<class AGuardian> SelectedGuardian4;
};
