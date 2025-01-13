#pragma once


#include "Blueprint/UserWidget.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "IngameUI.generated.h"

class UDebugMenu;
class UIngameMenu;
class UAbilityBar;
class UAbilitySlot;
class UAbilityImage;
class AGS_Ingame;
class UBuildingMenu;
class UButton;
class UGuardianInfo;
class UTextBlock;
class ACombat;
class UClickedInfo;
class USpinBox;

UCLASS(Blueprintable)
class GOTA_API UIngameUI : public UUserWidget
{
	GENERATED_BODY()

	// ------------------------------- LifeCycle -------------------------------
protected:
	virtual void NativeConstruct() override;

	// ------------------------------- Utility -------------------------------
public:
	void HoverActor(AActor* Actor);
	
	void HandleEscapePressed();

	// ------------------------------- Ingame Menu -------------------------------
private:
	UPROPERTY(meta = (BindWidget))
	UIngameMenu* IngameMenu;

	
public:
	void ToggleMenu();

	// ------------------------------- Guardian Info -------------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* WBP_GuardianInfo1;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* WBP_GuardianInfo2;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* WBP_GuardianInfo3;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* WBP_GuardianInfo4;

private:
	UFUNCTION()
	void RefreshGuardianWidgets(AGS_Ingame* GameState);

	// ------------------------------- Click Info -------------------------------
public:
	UFUNCTION(BlueprintCallable)
	void ClickActor(AActor* Actor);

protected:
	UPROPERTY(meta = (BindWidget))
	UClickedInfo* WBP_ClickedInfo;

	// ------------------------------- Build Menu -------------------------------
public:
	UFUNCTION()
	void ToggleBuildMenu();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Build;

	UPROPERTY(meta = (BindWidget))
	UBuildingMenu* WBP_BuildMenu;

private:
	void CloseBuildMenu();
	void OpenBuildMenu();

	// ------------------------------- Game Ended -------------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TXT_GameEnding;

private:
	UFUNCTION()
	void OnGameEnding(const EGameEnding Ending, const FString& EndingMessage);

	// ------------------------------- Ability -------------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UAbilityBar* WBP_AbilityBar;

public:
	TArray<UAbilitySlot*> GetAbilityBarSlots();

	// ------------------------------- Debug Menu -------------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UDebugMenu* DebugMenu;

public:
	void ToggleDebugMenu();
};
