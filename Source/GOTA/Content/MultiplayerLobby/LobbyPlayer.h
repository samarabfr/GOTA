// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LobbyPlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ULobbyPlayer : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

public:
	// selected guardian
	UPROPERTY(BlueprintReadWrite, Category="LobbyPlayer", ReplicatedUsing=OnRep_SelectedGuardian, BlueprintSetter=SetSelectedGuardian)
	TSubclassOf<class AGuardian> SelectedGuardian;

	UFUNCTION(BlueprintSetter)
	void SetSelectedGuardian(TSubclassOf<class AGuardian> NewSelectedGuardian);

	UFUNCTION(BlueprintCallable)
	void OnRep_SelectedGuardian();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedGuardianChangedSignature, TSubclassOf<class AGuardian>, NewSelectedGuardian);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnSelectedGuardianChangedSignature OnSelectedGuardianChanged;

	// Player state
	UPROPERTY(BlueprintReadWrite, Category="LobbyPlayer", ReplicatedUsing=OnRep_PlayerState, BlueprintSetter=SetPlayerState)
	APlayerState* PlayerState;

	UFUNCTION(BlueprintSetter)
	void SetPlayerState(APlayerState* NewPlayerState);

	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerState();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, APlayerState*, NewPlayerState);

	UPROPERTY(BlueprintAssignable, Category="GOTALobbyGameState")
	FOnPlayerStateChangedSignature OnPlayerStateChanged;
};
