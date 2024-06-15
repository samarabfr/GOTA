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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Players
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayersChangedSignature);
	
	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FPlayersChangedSignature OnPlayersChanged;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayersChanged();

	// Island Radius
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=IslandRadiusOnRep , Category="GOTAGameInstance")
	int32 IslandRadius;

	UFUNCTION(BlueprintCallable)
	void IslandRadiusOnRep(int32 NewIslandRadius);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIslandRadiusChangedSignature, int32, NewIslandRadius);

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FOnIslandRadiusChangedSignature OnIslandRadiusChanged;
};
