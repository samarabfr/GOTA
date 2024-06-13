// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GOTAPlayerState.generated.h"

UCLASS()
class GOTA_API AGOTAPlayerState : public APlayerState
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "PlayerState")
	int32 GOTAPlayerID = -1;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "PlayerState")
	void SetPlayerID(int32 NewPlayerID);
};
