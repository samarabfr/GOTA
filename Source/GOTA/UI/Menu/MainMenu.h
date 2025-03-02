#pragma once

#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UMultiplayerMenu;
class UVerticalBox;
class UOptions;
class UButton;

UCLASS()
class GOTA_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------
private:
	virtual void NativeConstruct() override;

	// ------------------- Utility -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_SinglePlayer;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Multiplayer;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Settings;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_SelfPlay;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Quit;

	UPROPERTY(meta = (BindWidget))
	UOptions* WBP_Options;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Menu;
	
	UPROPERTY(meta = (BindWidget))
	UMultiplayerMenu* WBP_MultiplayerMenu;

private:
	UFUNCTION()
	void StartSinglePlayer();
	
	UFUNCTION()
	void OpenMultiplayerMenu();

	UFUNCTION()
	void OpenSettings();
	
	UFUNCTION()
	void StartSelfPlay();
	
	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitGame();
};
