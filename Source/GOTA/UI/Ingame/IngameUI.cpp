#include "IngameUI.h"

#include "AbilityBar.h"
#include "BuildingMenu.h"
#include "ClickedInfo.h"
#include "GuardianInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

// ------------------------------- LifeCycle -------------------------------

void UIngameUI::NativeConstruct()
{
	Super::NativeConstruct();
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->OnGameEnding.AddDynamic(this, &UIngameUI::OnGameEnding);
	GameState->OnGuardiansChanged.AddDynamic(this, &UIngameUI::RefreshGuardianWidgets);
	RefreshGuardianWidgets(GameState);
	BTN_Build->OnPressed.AddDynamic(this, &UIngameUI::ToggleBuildMenu);
}

// ------------------------------- Utility -------------------------------

void UIngameUI::HoverActor(AActor* Actor)
{
}

// ------------------------------- Guardian Info -------------------------------

void UIngameUI::RefreshGuardianWidgets(AGS_Ingame* GameState)
{
	WBP_GuardianInfo1->SetGuardian(GameState->GetGuardian(0));
	WBP_GuardianInfo2->SetGuardian(GameState->GetGuardian(1));
	WBP_GuardianInfo3->SetGuardian(GameState->GetGuardian(2));
	WBP_GuardianInfo4->SetGuardian(GameState->GetGuardian(3));
}

// ------------------------------- Click Info -------------------------------

void UIngameUI::ClickActor(AActor* Actor)
{
	WBP_BuildMenu->SetVisibility(ESlateVisibility::Hidden);
	WBP_ClickedInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WBP_ClickedInfo->WatchActor(Actor);
}

// ------------------------------- Build Menu -------------------------------

void UIngameUI::ToggleBuildMenu()
{
	if (WBP_BuildMenu->IsVisible())
		CloseBuildMenu();
	else
		OpenBuildMenu();
}

void UIngameUI::CloseBuildMenu()
{
	WBP_BuildMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UIngameUI::OpenBuildMenu()
{
	WBP_BuildMenu->SetVisibility(ESlateVisibility::Visible);
	WBP_ClickedInfo->SetVisibility(ESlateVisibility::Hidden);
}

// ------------------------------- Game Ended -------------------------------

void UIngameUI::OnGameEnding(const EGameEnding Ending, const FString& EndingMessage)
{
	TXT_GameEnding->SetText(FText::FromString(EndingMessage));
}

TArray<UAbilitySlot*> UIngameUI::GetAbilityBarSlots()
{
	return WBP_AbilityBar->GetAbilitySlots();
}
