// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_Ingame.generated.h"


UCLASS()
class GOTA_API APS_Ingame : public APlayerState
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 GOTAPlayerID = -1;

	UPROPERTY(BlueprintReadOnly, Replicated)
	UGuardianSettings* SelectedGuardian;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SelectGuardian(UGuardianSettings* NewGuardian);
};
