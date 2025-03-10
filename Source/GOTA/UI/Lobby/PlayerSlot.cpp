#include "PlayerSlot.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "GOTA/GameplayFramework/PS_Ingame.h"
#include "GOTA/Guardian/GuardianSettings.h"

void UPlayerSlot::NativeConstruct()
{
	Super::NativeConstruct();

	for (const UGuardianSettings* GuardianSettings : Guardians)
	{
		GuardianSelection->AddOption(GuardianSettings->Name);
	}
	GuardianSelection->SetSelectedOption(GuardianSelection->GetOptionAtIndex(0));

	GuardianSelection->OnSelectionChanged.AddDynamic(this, &UPlayerSlot::OnSelectionChanged);
	OnSelectionChanged(GuardianSelection->GetOptionAtIndex(0), ESelectInfo::Direct);
}

void UPlayerSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (CachedPlayerState.IsValid())
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	if (!IsLocalPlayerState
		&& CachedPlayerState->SelectedGuardian
		&& CachedPlayerState->SelectedGuardian->Name != GuardianSelection->GetSelectedOption())
	{
		GuardianSelection->SetSelectedOption(CachedPlayerState->SelectedGuardian->Name);
	}
}

void UPlayerSlot::SetPlayerState(APS_Ingame* PlayerState)
{
	CachedPlayerState = PlayerState;
	if (!PlayerState)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	PlayerName->SetText(FText::FromString(PlayerState->GetPlayerName()));
	if (GetWorld()->GetFirstPlayerController()->PlayerState == PlayerState)
	{
		// This is the PlayerSlot of the local player
		IsLocalPlayerState = true;
		GuardianSelection->SetIsEnabled(true);
		PlayerState->SelectGuardian(GetSelectedGuardian());
	}
}

void UPlayerSlot::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!IsLocalPlayerState) return;
	CachedPlayerState->SelectGuardian(GetSelectedGuardian());
}

UGuardianSettings* UPlayerSlot::GetSelectedGuardian() const
{
	const FString Selected = GuardianSelection->GetSelectedOption();
	for (UGuardianSettings* GuardianSettings : Guardians)
	{
		if (GuardianSettings->Name == Selected) return GuardianSettings;
	}
	return nullptr;
}
