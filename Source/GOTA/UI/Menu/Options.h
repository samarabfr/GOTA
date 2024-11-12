#pragma once

#include "Blueprint/UserWidget.h"
#include "Options.generated.h"

class UGOTAGameUserSettings;
class UButton;
class UComboBoxString;

UCLASS()
class GOTA_API UOptions : public UUserWidget
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------
private:
	virtual void NativeConstruct() override;

	// ------------------- Utility -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Apply;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Back;

private:
	TWeakObjectPtr<UGOTAGameUserSettings> UserSettings;

	void FillComboBoxOptions();

	void RefreshEverything();
	
	UFUNCTION()
	void ApplyEverything();


	// ------------------- Screen -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_Resolutions;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ScreenMode;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_VSync;
	
private:
	TArray<FIntPoint> Resolutions;

	void FillResolutionsArray();

	void RefreshResolution();
	void ApplyResolution();

	void RefreshScreenMode();
	void ApplyScreenMode();
	
	void RefreshVsync();
	void ApplyVSync();
	
	// ------------------- Quality -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ResolutionScale;

	// ------------------- Shadows -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ShadowQuality;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_CSMShadows;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_DFShadows;

	// ------------------- Anti Aliasing -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_AntiAliasingType;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_AntiAliasingQuality;
	
};
