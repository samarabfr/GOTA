// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_Ingame.generated.h"

UCLASS()
class GOTA_API AGM_Ingame : public AGameMode
{
	GENERATED_BODY()
	
public:
virtual void PostLogin(APlayerController* NewPlayer) override;
	
	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="GameMode")
	void CreateMap();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="GameMode")
	void CreateFactions();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="GameMode")
	void CreateGuardians();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="GameMode")
	void InitialPlayerControllerPossession();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintImplementableEvent, Category="GameMode")
	void StartGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintImplementableEvent, Category="GameMode")
	void Init();
};