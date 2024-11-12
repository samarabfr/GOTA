#pragma once

#include "Blueprint/UserWidget.h"
#include "IngameMenu.generated.h"

class UVerticalBox;
class UCanvasPanel;
class UOptions;
class UButton;

UCLASS()
class GOTA_API UIngameMenu : public UUserWidget
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------
private:
	virtual void NativeConstruct() override;

	// ------------------- Utility -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Resume;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Settings;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Quit;

	UPROPERTY(meta = (BindWidget))
	UOptions* WBP_Options;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CP_Container;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Menu;

	UFUNCTION(BlueprintCallable)
	void Toggle();
	
	UFUNCTION()
	void Open();
	
	UFUNCTION()
	void Close();
	
private:
	UFUNCTION()
	void OpenOptions();

	UFUNCTION()
	void CloseOptions();
	
	UFUNCTION()
	void QuitGame();
};
