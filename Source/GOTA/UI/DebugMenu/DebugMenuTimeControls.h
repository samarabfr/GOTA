#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenuTimeControls.generated.h"

class ADaytimeManager;
class UEditableTextBox;
class USlider;
class UButton;

UCLASS()
class GOTA_API UDebugMenuTimeControls : public UUserWidget
{
	GENERATED_BODY()

	// -------------------------------------------- LifeCycle --------------------------------------------
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// -------------------------------------------- Utility --------------------------------------------
private:
	TWeakObjectPtr<ADaytimeManager> DaytimeManager;

	UFUNCTION()
	static void ValidateFloatText(const FText& InText, FString& OutString);

	// -------------------------------------------- Daytime --------------------------------------------

	// Morning, Midday, Evening, Night
	const TArray<float> TimeMarkers = {1.0f, 5.0f, 9.0f, 12.5f};

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Morning;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Midday;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Evening;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Night;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Skip;

	UFUNCTION()
	void SetTimeMorning();

	UFUNCTION()
	void SetTimeMidday();

	UFUNCTION()
	void SetTimeEvening();

	UFUNCTION()
	void SetTimeNight();

	UFUNCTION()
	void SkipToNextTime();
	
	// -------------------------------------------- Daytime Slider --------------------------------------------

	UPROPERTY(meta = (BindWidget))
	USlider* Slider_Daytime;

	bool bIsSliderMouseCaptured = false;

	UFUNCTION()
	void SetSliderMouseCapturedTrue() { bIsSliderMouseCaptured = true; }

	UFUNCTION()
	void SetSliderMouseCapturedFalse() { bIsSliderMouseCaptured = false; }

	UFUNCTION()
	void SetDaytime(const float InValue);

	// -------------------------------------------- Daytime Speed --------------------------------------------

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Stop;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Normal;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Fast;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TB_Speed;

	UFUNCTION()
	void SetDaytimeSpeedZero();

	UFUNCTION()
	void SetDaytimeSpeedOne();

	UFUNCTION()
	void SetDaytimeSpeedTwenty();

	UFUNCTION()
	void SetDaytimeSpeedCustom(const FText& Text, const ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void ValidateDaytimeSpeedTextBox(const FText& InText);

	// -------------------------------------------- Game Speed --------------------------------------------

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_GameStop;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_GameNormal;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_GameFast;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TB_GameSpeed;

	UFUNCTION()
	void SetGameDilationZero();

	UFUNCTION()
	void SetGameDilationOne();

	UFUNCTION()
	void SetGameDilationFive();

	UFUNCTION()
	void SetGameDilationCustom(const FText& Text, const ETextCommit::Type CommitMethod);

	UFUNCTION()
	void ValidateGameSpeedTextBox(const FText& InText);
};
