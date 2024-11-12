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
public:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Back;

private:
	TWeakObjectPtr<UGOTAGameUserSettings> UserSettings;

	void FillComboBoxOptions();

	void RefreshEverything();

	void RegisterDelegates();
	
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
	UFUNCTION()
	void ApplyResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshScreenMode();
	UFUNCTION()
	void ApplyScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshVsync();
	UFUNCTION()
	void ApplyVSync(FString SelectedItem, ESelectInfo::Type SelectionType);

	// ------------------- Quality -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ResolutionScale;

private:
	void RefreshResolutionScale();
	UFUNCTION()
	void ApplyResolutionScale(FString SelectedItem, ESelectInfo::Type SelectionType);

	// ------------------- Shadows -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ShadowQuality;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_CSMShadows;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_DFShadows;

private:
	void RefreshShadowQuality();
	UFUNCTION()
	void ApplyShadowQuality(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshCSMShadows();
	UFUNCTION()
	void ApplyCSMShadows(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshDFShadows();
	UFUNCTION()
	void ApplyDFShadows(FString SelectedItem, ESelectInfo::Type SelectionType);

	// ------------------- Anti Aliasing -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_AntiAliasingType;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_AntiAliasingQuality;

private:
	void RefreshAntiAliasingType();
	UFUNCTION()
	void ApplyAntiAliasingType(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshAntiAliasingQuality();
	UFUNCTION()
	void ApplyAntiAliasingQuality(FString SelectedItem, ESelectInfo::Type SelectionType);
};
