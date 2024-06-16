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
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_IslandRadius , Category="GOTALobbyGameState")
	int32 IslandRadius;

	UFUNCTION(BlueprintCallable)
	void OnRep_IslandRadius();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIslandRadiusChangedSignature, int32, NewIslandRadius);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnIslandRadiusChangedSignature OnIslandRadiusChanged;

	// Natives Count
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_NativesCount , Category="GOTALobbyGameState")
	int32 NativesCount;

	UFUNCTION(BlueprintCallable)
	void OnRep_NativesCount();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNativesCountChangedSignature, int32, NewNativesCount);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnNativesCountChangedSignature OnNativesCountChanged;

	// Colonists Count
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_ColonistsCount , Category="GOTALobbyGameState")
	int32 ColonistsCount;

	UFUNCTION(BlueprintCallable)
	void OnRep_ColonistsCount();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColonistsCountChangedSignature, int32, NewColonistsCount);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnColonistsCountChangedSignature OnColonistsCountChanged;
};
