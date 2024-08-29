#include "PlayerSlot.h"

void UPlayerSlot::NativeConstruct()
{
	Super::NativeConstruct();

	for (UGuardianDataAsset* GuardianDataAsset : Guardians)
	{
		GuardianSelection->AddOption(GuardianDataAsset->Name);
	}
	GuardianSelection->SetSelectedOption(GuardianSelection->GetOptionAtIndex(0));

	GuardianSelection->OnSelectionChanged.AddDynamic(this, &UPlayerSlot::OnSelectionChanged);
	OnSelectionChanged(GuardianSelection->GetOptionAtIndex(0), ESelectInfo::Direct);
}

void UPlayerSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!CachedPlayerState)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	SetVisibility(ESlateVisibility::Visible);

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

UGuardianDataAsset* UPlayerSlot::GetSelectedGuardian() const
{
	FString Selected = GuardianSelection->GetSelectedOption();
	for (UGuardianDataAsset* GuardianDataAsset : Guardians)
	{
		if (GuardianDataAsset->Name == Selected) return GuardianDataAsset;
	}
	return nullptr;
}
