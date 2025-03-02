// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/PlayerController.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingPlacer.h"
#include "GOTA/CoreSystems/Guardian/AbilitySettings.h"
#include "PC_Ingame.generated.h"

class UInputDataAsset;
class UAbilitySlot;
class AAbility;
class AAbilityIndicator;
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

	// ----------------------------------------- Replication Setup -----------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// -------------------------------------------- Lifecycle --------------------------------------------
public:
	virtual void BeginPlay() override;

	virtual void S_Init();

	virtual void C_Init();

	// -------------------------------------------- Utility --------------------------------------------
private:
	UPROPERTY()
	ADistanceUtils* DistanceUtils;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityIndicator> AbilityIndicatorClass;

	UPROPERTY(Replicated)
	TWeakObjectPtr<AAbilityIndicator> AbilityIndicator;

	void S_SetAbilityIndicator(AAbilityIndicator* NewAbilityIndicator);

	// -------------------------UI Stuff------------------------

private:
	UPROPERTY(BlueprintGetter=GetIngameUI, BlueprintSetter=SetIngameUI)
	UIngameUI* IngameUI;

public:
	UFUNCTION(BlueprintGetter)
	UIngameUI* GetIngameUI() const { return IngameUI; }

	UFUNCTION(BlueprintSetter)
	void SetIngameUI(UIngameUI* InIngameUI) { IngameUI = InIngameUI; }

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

	// -------------------------------------------- Guardian --------------------------------------------
private:
	UPROPERTY(ReplicatedUsing=OnRep_Guardian)
	AGuardian* Guardian;

	void S_SetGuardian(AGuardian* NewGuardian);

	UFUNCTION()
	void OnRep_Guardian();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuardianChangedSignature, AGuardian*, NewGuardian);

	UFUNCTION()
	void InitDistanceUtils(AGuardian* _);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
public:
	FOnGuardianChangedSignature OnGuardianChanged;
	AGuardian* GetGuardian() { return Guardian; }

	// ---------------------- InteractionMode ----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuildingPlacer> BuildingPlacerClass;

public:
	void ClickActor();

	UPROPERTY(Replicated)
	ABuildingPlacer* BuildingPlacer;

public:
	void S_SetBuildingPlacer(ABuildingPlacer* NewBuildingPlacer);

	void StartPlacingBuilding(UBuildingSettings* Building);

	void StopPlacingBuilding();

private:
	void PlaceBuilding();

	// ---------------------------------------- Ability ----------------------------------------
private:
	UPROPERTY()
	TWeakObjectPtr<AAbility> CurrentlyTargeting;

	void ActivateCurrentlyTargetingAbility();

	void StartTargeting(AAbility* Ability);
	void CancelTargeting();

public:
	void ActivateAbility(FName SlotName);
	void ActivateAbility(UAbilitySlot* Slot);

	void LearnAbility(UAbilitySettings* AbilitySettings);

	// ------------------------------------------- MouseUtils -------------------------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMouseUtils> MouseUtilsClass;

	UPROPERTY(ReplicatedUsing=OnRep_MouseUtils)
	TWeakObjectPtr<AMouseUtils> MouseUtils;

	UFUNCTION()
	void OnRep_MouseUtils();

	void S_SetMouseUtils(AMouseUtils* NewMouseUtils);

	void MouseUtilsChanged();

	UFUNCTION()
	void HoverActorChanged(AActor* Actor);

	// ------------------------------------------- Input -------------------------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	UInputDataAsset* InputDataAsset;

	void LeftClick(const FInputActionInstance& Instance);

	void StartJump(const FInputActionInstance& Instance);
	void StopJump(const FInputActionInstance& Instance);

	bool bIsLookingAround = false;
	FVector2D MousePositionWhenStartingLookingAround;
	void LookAround(const FInputActionInstance& Instance);
	void StartLookingAround(const FInputActionInstance& Instance);
	void StopLookingAround(const FInputActionInstance& Instance);

	void HandleEscapePressed();
	void ToggleBuildMenu();
	void ToggleDebugMenu();

	void ActivateAbility1();
	void ActivateAbility2();
	void ActivateAbility3();
	void ActivateAbility4();
	void ActivateAbility5();
	void ActivateAbility6();
	void ActivateAbility7();
	void ActivateAbility8();

public:
	void InitInput();
};
