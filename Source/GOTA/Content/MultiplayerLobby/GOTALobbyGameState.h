// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayer.h"
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

	virtual void BeginPlay() override;
	AGOTALobbyGameState();

public:
	// Island Radius
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_IslandRadius , Category="GOTALobbyGameState", BlueprintSetter=SetIslandRadius)
	int32 IslandRadius;

	UFUNCTION(BlueprintSetter)
	void SetIslandRadius(int32 NewIslandRadius);

	UFUNCTION(BlueprintCallable)
	void OnRep_IslandRadius();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIslandRadiusChangedSignature, int32, NewIslandRadius);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnIslandRadiusChangedSignature OnIslandRadiusChanged;

	// Natives Count
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_NativesCount , Category="GOTALobbyGameState", BlueprintSetter=SetNativesCount)
	int32 NativesCount;

	UFUNCTION(BlueprintSetter)
	void SetNativesCount(int32 NewNativesCount);

	UFUNCTION(BlueprintCallable)
	void OnRep_NativesCount();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNativesCountChangedSignature, int32, NewNativesCount);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnNativesCountChangedSignature OnNativesCountChanged;

	// Colonists Count
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_ColonistsCount , Category="GOTALobbyGameState", BlueprintSetter=SetColonistsCount)
	int32 ColonistsCount;

	UFUNCTION(BlueprintSetter)
	void SetColonistsCount(int32 NewColonistsCount);

	UFUNCTION(BlueprintCallable)
	void OnRep_ColonistsCount();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColonistsCountChangedSignature, int32, NewColonistsCount);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnColonistsCountChangedSignature OnColonistsCountChanged;

	// Lobby players
	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTALobbyGameState")
	ULobbyPlayer* LobbyPlayer1;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTALobbyGameState")
	ULobbyPlayer* LobbyPlayer2;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTALobbyGameState")
	ULobbyPlayer* LobbyPlayer3;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTALobbyGameState")
	ULobbyPlayer* LobbyPlayer4;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetSelectedGuardian(TSubclassOf<class AGuardian> Guardian, ULobbyPlayer* LobbyPlayer);
};
