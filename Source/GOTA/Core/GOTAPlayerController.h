// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GOTAPlayerController.generated.h"

UCLASS()
class GOTA_API AGOTAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PlayerController")
	void InitUI();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintImplementableEvent, Category="PlayerController")
	void Init();
};