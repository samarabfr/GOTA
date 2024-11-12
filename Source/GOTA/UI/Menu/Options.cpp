#include "Options.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GOTAGameUserSettings.h"

// ------------------- LifeCycle -------------------

void UOptions::NativeConstruct()
{
	Super::NativeConstruct();
	UserSettings = Cast<UGOTAGameUserSettings>(UGameUserSettings::GetGameUserSettings());

	BTN_Apply->OnPressed.AddDynamic(this, &UOptions::ApplyEverything);

	FillResolutionsArray();

	FillComboBoxOptions();

	RefreshEverything();
}

// ------------------- Utility -------------------

void UOptions::FillComboBoxOptions()
{
	for (const FIntPoint Resolution : Resolutions)
	{
		CB_Resolutions->AddOption(FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y));
	}

	CB_ScreenMode->AddOption(TEXT("FullScreen"));
	CB_ScreenMode->AddOption(TEXT("Borderless FullScreen"));
	CB_ScreenMode->AddOption(TEXT("Windowed"));

	CB_VSync->AddOption(TEXT("On"));
	CB_VSync->AddOption(TEXT("Off"));
	
	CB_ResolutionScale->AddOption(TEXT("Native"));
	CB_ResolutionScale->AddOption(TEXT("75%"));
	CB_ResolutionScale->AddOption(TEXT("50%"));

	CB_ShadowQuality->AddOption(TEXT("High"));
	CB_ShadowQuality->AddOption(TEXT("Low"));

	CB_CSMShadows->AddOption(TEXT("On"));
	CB_CSMShadows->AddOption(TEXT("Off"));

	CB_DFShadows->AddOption(TEXT("On"));
	CB_DFShadows->AddOption(TEXT("Off"));

	CB_AntiAliasingType->AddOption(TEXT("TSR"));
	CB_AntiAliasingType->AddOption(TEXT("TAA"));
	CB_AntiAliasingType->AddOption(TEXT("FXAA"));
	CB_AntiAliasingType->AddOption(TEXT("Off"));

	CB_AntiAliasingQuality->AddOption(TEXT("High"));
	CB_AntiAliasingQuality->AddOption(TEXT("Medium"));
	CB_AntiAliasingQuality->AddOption(TEXT("Low"));
}

void UOptions::RefreshEverything()
{
	RefreshResolution();
	RefreshScreenMode();
	RefreshVsync();
	

	//const float ResolutionScale = UserSettings->GetResolutionScaleNormalized();
	//	Slider_ResolutionScale->SetValue(ResolutionScale);

	const ECheckBoxState CSMShadows = UserSettings->GetCascadedShadowMapsEnabled()
		                                  ? ECheckBoxState::Checked
		                                  : ECheckBoxState::Unchecked;
	//CheckBox_CSMShadows->SetCheckedState(CSMShadows);

	const ECheckBoxState DFShadows = UserSettings->GetDistanceFieldShadowsEnabled()
		                                 ? ECheckBoxState::Checked
		                                 : ECheckBoxState::Unchecked;
	//CheckBox_DFShadows->SetCheckedState(DFShadows);
}

void UOptions::ApplyEverything()
{
	ApplyResolution();
	ApplyScreenMode();
	ApplyVSync();

	

	//const float ResolutionScale = Slider_ResolutionScale->GetValue();
	//UserSettings->SetResolutionScaleNormalized(ResolutionScale);

	//const bool CSMShadows = CheckBox_CSMShadows->GetCheckedState() == ECheckBoxState::Checked;
	//UserSettings->SetCascadedShadowMapsEnabled(CSMShadows);

	//const bool DFShadows = CheckBox_DFShadows->GetCheckedState() == ECheckBoxState::Checked;
	//UserSettings->SetDistanceFieldShadowsEnabled(DFShadows);

	UserSettings->ApplySettings(true);
}

// ------------------- Screen -------------------

void UOptions::FillResolutionsArray()
{
	FScreenResolutionArray ResolutionsRHI;
	RHIGetAvailableResolutions(ResolutionsRHI, false);
	for (int i = ResolutionsRHI.Num() - 1; i >= 0; --i)
	{
		FIntPoint Resolution = FIntPoint(ResolutionsRHI[i].Width, ResolutionsRHI[i].Height);
		if (!Resolutions.Contains(Resolution))
		{
			Resolutions.Add(Resolution);
		}
	}
}

void UOptions::RefreshResolution()
{
	const FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
	for (int i = 0; i < Resolutions.Num(); ++i)
	{
		if (Resolutions[i] == CurrentResolution)
		{
			CB_Resolutions->SetSelectedIndex(i);
			return;
		}
	}
	// Current Resolution is not in the ResolutionsArray... wierd, but maybe it will happen if it is set Custom
	// with start parameter
	CB_Resolutions->AddOption(TEXT("Unknown"));
	CB_Resolutions->SetSelectedOption(TEXT("Unknown"));
}

void UOptions::ApplyResolution()
{
	if (!Resolutions.IsValidIndex(CB_Resolutions->GetSelectedIndex())) return;

	const FIntPoint SelectedResolution = Resolutions[CB_Resolutions->GetSelectedIndex()];
	UserSettings->SetScreenResolution(SelectedResolution);
	UE_LOG(LogTemp, Warning, TEXT("Set Resolution to: %dx%d"), SelectedResolution.X, SelectedResolution.Y);
}

void UOptions::RefreshScreenMode()
{
	const EWindowMode::Type ScreenMode = UserSettings->GetFullscreenMode();
	CB_ScreenMode->SetSelectedIndex(ScreenMode);
}


void UOptions::ApplyScreenMode()
{
	const EWindowMode::Type WindowMode = static_cast<EWindowMode::Type>(CB_ScreenMode->GetSelectedIndex());
	UserSettings->SetFullscreenMode(WindowMode);
}

void UOptions::RefreshVsync()
{
	const bool VSyncEnabled = UserSettings->IsVSyncEnabled();
	CB_VSync->SetSelectedIndex(!VSyncEnabled);
}

void UOptions::ApplyVSync()
{
	const bool VSyncEnabled = CB_VSync->GetSelectedIndex() == 0;
	UserSettings->SetVSyncEnabled(VSyncEnabled);
}

// ------------------- Widgets -------------------
