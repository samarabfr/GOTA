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

	FillResolutionsArray();

	FillComboBoxOptions();

	RefreshEverything();

	RegisterDelegates();
}


// ------------------- Utility -------------------

void UOptions::FillComboBoxOptions()
{
	for (const FIntPoint Resolution : Resolutions)
	{
		CB_Resolutions->AddOption(FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y));
	}

	CB_ScreenMode->AddOption(TEXT("Fullscreen"));
	CB_ScreenMode->AddOption(TEXT("Borderless"));
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
	RefreshResolutionScale();

	RefreshShadowQuality();
	RefreshCSMShadows();
	RefreshDFShadows();

	RefreshAntiAliasingType();
	RefreshAntiAliasingQuality();
}

void UOptions::RegisterDelegates()
{
	CB_Resolutions->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyResolution);
	CB_ScreenMode->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyScreenMode);
	CB_VSync->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyVSync);
	CB_ResolutionScale->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyResolutionScale);

	CB_ShadowQuality->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyShadowQuality);
	CB_CSMShadows->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyCSMShadows);
	CB_DFShadows->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyDFShadows);

	CB_AntiAliasingType->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyAntiAliasingType);
	CB_AntiAliasingQuality->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyAntiAliasingQuality);
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
	if (CB_Resolutions->GetOptionCount() == Resolutions.Num())
		CB_Resolutions->AddOption(TEXT("Custom"));
	CB_Resolutions->SetSelectedIndex(Resolutions.Num());
}

void UOptions::ApplyResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!Resolutions.IsValidIndex(CB_Resolutions->GetSelectedIndex())) return;

	const FIntPoint SelectedResolution = Resolutions[CB_Resolutions->GetSelectedIndex()];
	UserSettings->SetScreenResolution(SelectedResolution);
	UserSettings->ApplySettings(true);
}

void UOptions::RefreshScreenMode()
{
	const EWindowMode::Type ScreenMode = UserSettings->GetFullscreenMode();
	CB_ScreenMode->SetSelectedIndex(ScreenMode);
}

void UOptions::ApplyScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const EWindowMode::Type WindowMode = static_cast<EWindowMode::Type>(CB_ScreenMode->GetSelectedIndex());
	UserSettings->SetFullscreenMode(WindowMode);
	UserSettings->ApplySettings(true);
}

void UOptions::RefreshVsync()
{
	const bool VSyncEnabled = UserSettings->IsVSyncEnabled();
	CB_VSync->SetSelectedIndex(!VSyncEnabled);
}

void UOptions::ApplyVSync(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const bool VSyncEnabled = CB_VSync->GetSelectedIndex() == 0;
	UserSettings->SetVSyncEnabled(VSyncEnabled);
	UserSettings->ApplySettings(true);
}


// ------------------- Quality -------------------

void UOptions::RefreshResolutionScale()
{
	const float ResolutionScale = UserSettings->GetResolutionScaleNormalized();

	if (FMath::IsNearlyEqual(ResolutionScale, 1.0f))
		CB_ResolutionScale->SetSelectedIndex(0);

	else if (FMath::IsNearlyEqual(ResolutionScale, 0.75f))
		CB_ResolutionScale->SetSelectedIndex(1);

	else if (FMath::IsNearlyEqual(ResolutionScale, 0.5f))
		CB_ResolutionScale->SetSelectedIndex(2);

	else
	{
		// In case ResolutionScale got set custom somehow
		if (CB_ResolutionScale->GetOptionCount() < 4)
			CB_ResolutionScale->AddOption(TEXT("Custom"));
		CB_ResolutionScale->SetSelectedIndex(3);
	}
}

void UOptions::ApplyResolutionScale(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (CB_ResolutionScale->GetSelectedIndex())
	{
	case 0:
		UserSettings->SetResolutionScaleNormalized(1.0f);
		break;

	case 1:
		UserSettings->SetResolutionScaleNormalized(0.75f);
		break;

	case 2:
		UserSettings->SetResolutionScaleNormalized(0.5f);

	default:
		return;
	}
	UserSettings->ApplySettings(true);
}


// ------------------- Shadows -------------------

void UOptions::RefreshShadowQuality()
{
	switch (UserSettings->GetShadowQuality())
	{
	case 3: // High
		CB_ShadowQuality->SetSelectedIndex(0);
		return;

	case 1: // Low
		CB_ShadowQuality->SetSelectedIndex(1);
		return;

	default:
		if (CB_ShadowQuality->GetOptionCount() == 2)
			CB_ShadowQuality->AddOption(TEXT("Custom"));
		CB_ShadowQuality->SetSelectedIndex(2);
		return;
	}
}

void UOptions::ApplyShadowQuality(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (CB_ShadowQuality->GetSelectedIndex())
	{
	case 0: // High
		UserSettings->SetShadowQuality(3);
		break;

	case 1: // Low
		UserSettings->SetShadowQuality(1);
		break;

	default:
		return;
	}
	UserSettings->ApplySettings(true);
}

void UOptions::RefreshCSMShadows()
{
	const bool CSMEnabled = UserSettings->IsCascadedShadowMapsEnabled();
	CB_CSMShadows->SetSelectedIndex(!CSMEnabled);
}

void UOptions::ApplyCSMShadows(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const bool CSMEnabled = CB_CSMShadows->GetSelectedIndex() == 0;
	UserSettings->SetCascadedShadowMapsEnabled(CSMEnabled);
	UserSettings->ApplySettings(true);

	// if shadows are Off, setting the quality is useless
	if (CB_DFShadows->GetSelectedIndex() == 1 && CB_CSMShadows->GetSelectedIndex() == 1)
		CB_ShadowQuality->SetIsEnabled(false);
	else
		CB_ShadowQuality->SetIsEnabled(true);
}

void UOptions::RefreshDFShadows()
{
	const bool DFEnabled = UserSettings->IsDistanceFieldShadowsEnabled();
	CB_DFShadows->SetSelectedIndex(!DFEnabled);
}

void UOptions::ApplyDFShadows(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const bool DFEnabled = CB_DFShadows->GetSelectedIndex() == 0;
	UserSettings->SetDistanceFieldShadowsEnabled(DFEnabled);
	UserSettings->ApplySettings(true);

	// if shadows are Off, setting the quality is useless
	if (CB_DFShadows->GetSelectedIndex() == 1 && CB_CSMShadows->GetSelectedIndex() == 1)
		CB_ShadowQuality->SetIsEnabled(false);
	else
		CB_ShadowQuality->SetIsEnabled(true);
}


// ------------------- Anti Aliasing -------------------

void UOptions::RefreshAntiAliasingType()
{
	switch (UserSettings->GetAntiAliasingType())
	{
	case 4: // TSR
		CB_AntiAliasingType->SetSelectedIndex(0);
		return;

	case 2: // TAA
		CB_AntiAliasingType->SetSelectedIndex(1);
		return;

	case 1: // FXAA
		CB_AntiAliasingType->SetSelectedIndex(2);
		return;

	case 0: // Off
		CB_AntiAliasingType->SetSelectedIndex(3);
		return;

	default:
		return;
	}
}

void UOptions::ApplyAntiAliasingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (CB_AntiAliasingType->GetSelectedIndex())
	{
	case 0: // TSR
		UserSettings->SetAntiAliasingType(4);
		break;

	case 1: // TAA
		UserSettings->SetAntiAliasingType(2);
		break;

	case 2: // FXAA
		UserSettings->SetAntiAliasingType(1);
		break;

	case 3: // Off
		UserSettings->SetAntiAliasingType(0);
		break;

	default:
		return;
	}
	UserSettings->ApplySettings(true);

	// if Anti Aliasing is Off, setting the quality is useless
	if (CB_AntiAliasingType->GetSelectedIndex() == 3)
		CB_AntiAliasingQuality->SetIsEnabled(false);
	else
		CB_AntiAliasingQuality->SetIsEnabled(true);
}

void UOptions::RefreshAntiAliasingQuality()
{
	switch (UserSettings->GetAntiAliasingQuality())
	{
	case 3: // High
		CB_AntiAliasingQuality->SetSelectedIndex(0);
		return;

	case 2: //Medium
		CB_AntiAliasingQuality->SetSelectedIndex(1);
		return;

	case 1: // Low
		CB_AntiAliasingQuality->SetSelectedIndex(2);

	default:
		if (CB_AntiAliasingQuality->GetOptionCount() == 3)
			CB_AntiAliasingQuality->AddOption("Custom");
		CB_AntiAliasingQuality->SetSelectedIndex(3);
	}
}

void UOptions::ApplyAntiAliasingQuality(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (CB_AntiAliasingQuality->GetSelectedIndex())
	{
	case 0: // High
		UserSettings->SetAntiAliasingQuality(3);
		break;

	case 1: // Medium
		UserSettings->SetAntiAliasingQuality(2);
		break;

	case 2: // Low
		UserSettings->SetAntiAliasingQuality(1);
		break;

	default:
		return;
	}
	UserSettings->ApplySettings(true);
}
