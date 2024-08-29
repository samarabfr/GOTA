// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat.h"
#include "GameFramework/PlayerController.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "PC_Ingame.generated.h"

UCLASS()
class GOTA_API APC_Ingame : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintCosmetic, Category="PlayerController")
	void InitUI();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintImplementableEvent, Category="PlayerController")
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintCosmetic, Category="PlayerController")
	void InitInput();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintCosmetic, Category="PlayerController")
	void PossessGuardian(AGuardian* Guardian);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintCosmetic, Category="PlayerController")
	void WatchCombat(ACombat* Combat);
};
