#include "DebugMenuAbilityEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GOTA/GameplayFramework/PC_Ingame.h"
#include "GOTA/Guardian/AbilityFramework/AbilitySettings.h"

FReply UDebugMenuAbilityEntry::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	APC_Ingame* PlayerController = GetWorld()->GetFirstPlayerController<APC_Ingame>();
	if (PlayerController)
	{
		PlayerController->LearnAbility(AbilitySettings.Get());
	}
	
	return FReply::Handled();
}

void UDebugMenuAbilityEntry::SetAbility(UAbilitySettings* NewAbilitySettings)
{
	AbilitySettings = NewAbilitySettings;
	if (AbilitySettings.IsValid())
	{
		AbilityImage->SetBrushFromTexture(AbilitySettings->GetIcon());
		AbilityName->SetText(FText::FromName(AbilitySettings->GetAbilityName()));
	}
}
