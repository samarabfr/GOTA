#include "Options.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/GameUserSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GOTAGameUserSettings.h"
#include "GOTA/UI/Widgets/ToggleButton.h"


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

	CB_ResolutionScale->AddOption(TEXT("Native"));
	CB_ResolutionScale->AddOption(TEXT("75%"));
	CB_ResolutionScale->AddOption(TEXT("50%"));

	CB_ShadowQuality->AddOption(TEXT("High"));
	CB_ShadowQuality->AddOption(TEXT("Low"));
	CB_ShadowQuality->AddOption(TEXT("Off"));

	CB_ShadowDistance->AddOption(TEXT("High"));
	CB_ShadowDistance->AddOption(TEXT("Medium"));
	CB_ShadowDistance->AddOption(TEXT("Low"));
	
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
	RefreshResolutionScale();
	
	RefreshVsync();
	RefreshUsingFPSLimit();
	RefreshFPSLimit();

	RefreshShadowQuality();
	RefreshShadowDistance();

	RefreshAntiAliasingType();
	RefreshAntiAliasingQuality();
}

void UOptions::RegisterDelegates()
{
	CB_Resolutions->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyResolution);
	CB_ScreenMode->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyScreenMode);
	CB_ResolutionScale->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyResolutionScale);

	TB_VSync->OnActiveChanged.AddDynamic(this, &UOptions::ApplyVSync);
	TB_UsingFPSLimit->OnActiveChanged.AddDynamic(this, &UOptions::ApplyUsingFPSLimit);
	ETXT_FPSLimit->OnTextChanged.AddDynamic(this, &UOptions::ValidateFPSLimitInput);
	ETXT_FPSLimit->OnTextCommitted.AddDynamic(this, &UOptions::ApplyFPSLimit);

	CB_ShadowQuality->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyShadowQuality);
	CB_ShadowDistance->OnSelectionChanged.AddDynamic(this, &UOptions::ApplyShadowDistance);

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


// ------------------- FPS -------------------

void UOptions::RefreshVsync()
{
	const bool VSyncEnabled = UserSettings->IsVSyncEnabled();
	TB_VSync->SetIsActive(VSyncEnabled, true);
}

void UOptions::ApplyVSync(bool NewActive)
{
	UserSettings->SetVSyncEnabled(NewActive);
	UserSettings->ApplySettings(true);
}

void UOptions::RefreshUsingFPSLimit()
{
	const bool UsingFPSLimit = UserSettings->IsUsingFPSLimit();
	TB_UsingFPSLimit->SetIsActive(UsingFPSLimit, true);
}

void UOptions::ApplyUsingFPSLimit(bool NewActive)
{
	UserSettings->SetIsUsingFPSLimit(NewActive);
	UserSettings->ApplySettings(true);
}

void UOptions::RefreshFPSLimit()
{
	const int32 FPSLimit = UserSettings->GetFPSLimit();
	ETXT_FPSLimit->SetText(FText::AsNumber(FPSLimit));
}

void UOptions::ValidateFPSLimitInput(const FText& Text)
{
	FString InputString = Text.ToString();
	FString ValidString;

	for (const TCHAR& Char : InputString)
	{
		if (FChar::IsDigit(Char))
		{
			ValidString.AppendChar(Char);
		}
	}
	ETXT_FPSLimit->SetText(FText::FromString(ValidString));
}

void UOptions::ApplyFPSLimit(const FText& Text, ETextCommit::Type CommitMethod)
{
	const int32 InputFPS = FCString::Atoi(*Text.ToString());
	const int32 ClampedFPS = FMath::Clamp(InputFPS, 15, 999);
	ETXT_FPSLimit->SetText(FText::AsNumber(ClampedFPS));
	UserSettings->SetFPSLimit(ClampedFPS);
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

	case 2: // Low
		CB_ShadowQuality->SetSelectedIndex(1);
		return;

	case 1: // Off
		CB_ShadowQuality->SetSelectedIndex(2);
		return;

	default:
		if (CB_ShadowQuality->GetOptionCount() == 3)
			CB_ShadowQuality->AddOption(TEXT("Custom"));
		CB_ShadowQuality->SetSelectedIndex(3);
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
		UserSettings->SetShadowQuality(2);
		break;

	case 2: // Off
		UserSettings->SetShadowQuality(1);
		break;

	default:
		return;
	}
	UserSettings->ApplySettings(true);
}

void UOptions::RefreshShadowDistance()
{
	const float DistanceFactor = UserSettings->GetShadowDistanceFactor();
	
	if (FMath::IsNearlyEqual(DistanceFactor, 1.0f)) // High
		CB_ShadowDistance->SetSelectedIndex(0);

	else if (FMath::IsNearlyEqual(DistanceFactor, 0.65f)) // Medium
		CB_ShadowDistance->SetSelectedIndex(1);

	else if (FMath::IsNearlyEqual(DistanceFactor, 0.4f)) // Low
		CB_ShadowDistance->SetSelectedIndex(2);

	else
	{
		// In case ResolutionScale got set custom somehow
		if (CB_ShadowDistance->GetOptionCount() == 3)
			CB_ShadowDistance->AddOption(TEXT("Custom"));
		CB_ShadowDistance->SetSelectedIndex(3);
	}
}

void UOptions::ApplyShadowDistance(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (CB_ShadowDistance->GetSelectedIndex())
	{
	case 0: // High
		UserSettings->SetShadowDistanceFactor(1.0f);
		break;

	case 1: // Medium
		UserSettings->SetShadowDistanceFactor(0.65f);
		break;

	case 2: // Low
		UserSettings->SetShadowDistanceFactor(0.4f);
		break;

	default:
		return;
	}
	UserSettings->ApplySettings(true);
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
