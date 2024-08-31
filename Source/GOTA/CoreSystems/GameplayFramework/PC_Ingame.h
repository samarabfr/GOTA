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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	ADistanceUtils* DistanceUtils;

	UPROPERTY(ReplicatedUsing=OnRep_Guardian, BlueprintGetter=GetGuardian)
	AGuardian* Guardian;

	UPROPERTY(ReplicatedUsing=OnRep_MouseUtils, BlueprintGetter=GetMouseUtils)
	AMouseUtils* MouseUtils;
	
	// ---------------------------------------------------------
	// Setup
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PlayerController")
	void InitInput();
	
	virtual void OnPossess(APawn* InPawn) override;
	
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
	void BindToMouseUtils(AMouseUtils* MouseUtils_);

	// ---------------------------------------------------------
	// Getter & Setter

	UFUNCTION(BlueprintGetter)
	AGuardian* GetGuardian();
	
	void SetGuardian(AGuardian* Guardian_);
	
	UFUNCTION()
	void OnRep_Guardian();

	void GuardianChanged();
	
	UFUNCTION(BlueprintGetter)
	AMouseUtils* GetMouseUtils();
	
	void SetMouseUtils(AMouseUtils* MouseUtils_);

	UFUNCTION()
	void OnRep_MouseUtils();

	void MouseUtilsChanged();
};
