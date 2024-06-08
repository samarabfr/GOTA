// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GOTALobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API AGOTALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayersChangedSignature);
	
	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FPlayersChangedSignature OnPlayersChanged;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayersChanged();
};
