#include "DebugMenuTimeControls.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "GOTA/CoreSystems/GameplayFramework/DaytimeManager.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

// -------------------------------------------- LifeCycle --------------------------------------------

void UDebugMenuTimeControls::NativeConstruct()
{
	Super::NativeConstruct();
	if (const AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>())
	{
		DaytimeManager = GameState->DaytimeManager;
	}
	if (DaytimeManager.IsValid())
	{
		Slider_Daytime->SetMaxValue(DaytimeManager->GetFullDayLength());
	}

	BTN_Morning->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeMorning);
	BTN_Midday->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeMidday);
	BTN_Evening->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeEvening);
	BTN_Night->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeNight);

	BTN_Skip->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SkipToNextTime);

	BTN_Stop->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetSpeedZero);
	BTN_Normal->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetSpeedOne);
	BTN_Fast->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetSpeedTwenty);

	TB_Speed->OnTextCommitted.AddDynamic(this, &UDebugMenuTimeControls::SetSpeedCustom);
	TB_Speed->OnTextChanged.AddDynamic(this, &UDebugMenuTimeControls::ValidateSpeedTextBox);

	Slider_Daytime->OnValueChanged.AddDynamic(this, &UDebugMenuTimeControls::SetDaytime);
	Slider_Daytime->OnMouseCaptureBegin.AddDynamic(this, &UDebugMenuTimeControls::SetSliderMouseCapturedTrue);
	Slider_Daytime->OnMouseCaptureEnd.AddDynamic(this, &UDebugMenuTimeControls::SetSliderMouseCapturedFalse);
}

void UDebugMenuTimeControls::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DaytimeManager.IsValid())
	{
		if (!bIsSliderMouseCaptured)
		{
			Slider_Daytime->SetValue(DaytimeManager->GetTime());
		}
		if (!TB_Speed->HasKeyboardFocus())
		{
			TB_Speed->SetText(FText::AsNumber(DaytimeManager->GetDayTimeSpeed()));
		}
	}
}

// -------------------------------------------- Utility --------------------------------------------

// -------------------------------------------- Daytime --------------------------------------------

void UDebugMenuTimeControls::SetTimeMorning()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[0]);
}

void UDebugMenuTimeControls::SetTimeMidday()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[1]);
}

void UDebugMenuTimeControls::SetTimeEvening()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[2]);
}

void UDebugMenuTimeControls::SetTimeNight()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[3]);
}

void UDebugMenuTimeControls::SkipToNextTime()
{
	if (!DaytimeManager.IsValid()) return;

	const float CurrentTime = DaytimeManager->GetTime();

	for (const float Time : TimeMarkers)
	{
		if (CurrentTime < Time)
		{
			DaytimeManager->SetTime(Time);
			return;
		}
	}
	DaytimeManager->SetTime(TimeMarkers[0]);
}

// -------------------------------------------- Daytime Slider --------------------------------------------

void UDebugMenuTimeControls::SetDaytime(const float InValue)
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(InValue);
}

// -------------------------------------------- Daytime Speed --------------------------------------------

void UDebugMenuTimeControls::SetSpeedZero()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(0.0f);
}

void UDebugMenuTimeControls::SetSpeedOne()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(1.0f);
}

void UDebugMenuTimeControls::SetSpeedTwenty()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(20.0f);
}

void UDebugMenuTimeControls::SetSpeedCustom(const FText& Text, const ETextCommit::Type CommitMethod)
{
	const float Speed = FCString::Atof(*Text.ToString());
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(Speed);
}

void UDebugMenuTimeControls::ValidateSpeedTextBox(const FText& InText)
{
	FString FilteredText;
	FString InputString = InText.ToString();
	bool FoundDot = false;

	for (int32 i = 0; i < InputString.Len(); i++)
	{
		const TCHAR Char = InputString[i];
		if (FChar::IsDigit(Char))
		{
			FilteredText.AppendChar(Char);
		}
		if ((Char == '.') && !FoundDot)
		{
			FilteredText.AppendChar(Char);
			FoundDot = true;
		}
	}
	TB_Speed->SetText(FText::FromString(FilteredText));
}
