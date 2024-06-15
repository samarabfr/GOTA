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

	// Natives Count
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=NativesCountOnRep , Category="GOTAGameState")
	int32 NativesCount;

	UFUNCTION(BlueprintCallable)
	void NativesCountOnRep(int32 NewNativesCount);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNativesCountChangedSignature, int32, NewNativesCount);

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FOnNativesCountChangedSignature OnNativesCountChanged;

	// Island Radius
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=ColonistsCountOnRep , Category="GOTAGameState")
	int32 ColonistsCount;

	UFUNCTION(BlueprintCallable)
	void ColonistsCountOnRep(int32 NewColonistsCount);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColonistsCountChangedSignature, int32, NewColonistsCount);

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FOnColonistsCountChangedSignature OnColonistsCountChanged;
};
