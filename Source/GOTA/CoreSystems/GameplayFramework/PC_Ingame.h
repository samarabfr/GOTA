// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat.h"
#include "GameFramework/PlayerController.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Utility/DistanceUtils.h"
#include "PC_Ingame.generated.h"

UCLASS()
class GOTA_API APC_Ingame : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	ADistanceUtils* DistanceUtils;

	UPROPERTY(BlueprintGetter=GetGuardian)
	AGuardian* Guardian;

	// ---------------------------------------------------------
	// Setup
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PlayerController")
	void InitInput();

	UFUNCTION(BlueprintCallable, Category="PlayerController")
	void PossessGuardian(AGuardian* NewGuardian);

	// ---------------------------------------------------------
	// UI Stuff

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PlayerController")
	void WatchCombat(ACombat* Combat);

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void CreateLobbyUI();

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void RemoveLobbyUI();

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void CreateLoadingUI();

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void RemoveLoadingUI();

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void CreateIngameUI();

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void BindToMouseUtils(AMouseUtils* MouseUtils);

	// ---------------------------------------------------------
	// Getter & Setter

	UFUNCTION(BlueprintGetter)
	AGuardian* GetGuardian();
};
