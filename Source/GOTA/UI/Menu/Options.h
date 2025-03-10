#pragma once

#include "Blueprint/UserWidget.h"
#include "Options.generated.h"

class UEditableTextBox;
class UToggleButton;
class UGotaGameUserSettings;
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
	TWeakObjectPtr<UGotaGameUserSettings> UserSettings;

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
	UComboBoxString* CB_ResolutionScale;

private:
	TArray<FIntPoint> Resolutions;

	void FillResolutionsArray();

	void RefreshResolution();
	UFUNCTION()
	void ApplyResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshScreenMode();
	UFUNCTION()
	void ApplyScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshResolutionScale();
	UFUNCTION()
	void ApplyResolutionScale(FString SelectedItem, ESelectInfo::Type SelectionType);

	// ------------------- FPS -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UToggleButton* TB_VSync;

	UPROPERTY(meta = (BindWidget))
	UToggleButton* TB_UsingFPSLimit;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ETXT_FPSLimit;

private:
	void RefreshVsync();
	UFUNCTION()
	void ApplyVSync(bool NewActive);

	void RefreshUsingFPSLimit();
	UFUNCTION()
	void ApplyUsingFPSLimit(bool NewActive);

	void RefreshFPSLimit();
	UFUNCTION()
	void ValidateFPSLimitInput(const FText& Text);
	UFUNCTION()
	void ApplyFPSLimit(const FText& Text, ETextCommit::Type CommitMethod);

	// ------------------- Shadows -------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ShadowQuality;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_ShadowDistance;

private:
	void RefreshShadowQuality();
	UFUNCTION()
	void ApplyShadowQuality(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshShadowDistance();
	UFUNCTION()
	void ApplyShadowDistance(FString SelectedItem, ESelectInfo::Type SelectionType);
	

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

	// -------------------------------------- Sharpen --------------------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_Sharpen;

private:
	void RefreshSharpen();
	UFUNCTION()
	void ApplySharpen(FString SelectedItem, ESelectInfo::Type SelectionType);
};
