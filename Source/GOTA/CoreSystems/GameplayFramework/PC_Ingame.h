// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/PlayerController.h"
#include "PC_Ingame.generated.h"

class UBuildingSettings;
struct FInputActionInstance;
class AMouseUtils;
class ADistanceUtils;
class AGuardian;
class UIngameUI;

UCLASS()
class GOTA_API APC_Ingame : public APlayerController
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	ADistanceUtils* DistanceUtils;

	// ---------------------------------------------------------
	// Setup
	virtual void BeginPlay() override;

	// -------------------------UI Stuff------------------------

private:
	UPROPERTY(BlueprintGetter=GetIngameUI, BlueprintSetter=SetIngameUI)
	UIngameUI* IngameUI;

public:
	UFUNCTION(BlueprintGetter)
	UIngameUI* GetIngameUI() const { return IngameUI; }

	UFUNCTION(BlueprintSetter)
	void SetIngameUI(UIngameUI* InIngameUI) { IngameUI = InIngameUI; }

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

	// ------------------------ Guardian ------------------------
public:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(ReplicatedUsing=GuardianChanged)
	AGuardian* Guardian;

public:
	AGuardian* GetGuardian() { return Guardian; }

private:
	void SetGuardian(AGuardian* NewGuardian);

	UFUNCTION()
	void GuardianChanged();

	// ---------------------- InteractionMode ----------------------

	void ClickActor();

	bool bIsPlacingBuilding = false;
	
	UPROPERTY()
	UBuildingSettings* BuildingToPlace = nullptr;
public:
	void StartPlacingBuilding(UBuildingSettings* Building);

	void StopPlacingBuilding();
private:
	void PlaceBuilding();

	// ----------------------- Input -----------------------
public:
	void InitInput();

	UPROPERTY(ReplicatedUsing=MouseUtilsChanged)
	AMouseUtils* MouseUtils;

	AMouseUtils* GetMouseUtils() const { return MouseUtils; }

	void SetMouseUtils(AMouseUtils* NewMouseUtils);

	UFUNCTION()
	void MouseUtilsChanged();

	UFUNCTION()
	void OnHoverActorChanged(AActor* Actor);

private:
	void LeftClick(const FInputActionInstance& Instance);

	void StartJump(const FInputActionInstance& Instance);
	void StopJump(const FInputActionInstance& Instance);

	bool bIsLookingAround = false;
	FVector2D MousePositionWhenStartingLookingAround;
	void LookAround(const FInputActionInstance& Instance);
	void StartLookingAround(const FInputActionInstance& Instance);
	void StopLookingAround(const FInputActionInstance& Instance);
};
