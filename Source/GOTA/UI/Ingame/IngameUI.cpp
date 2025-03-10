#include "IngameUI.h"

#include "AbilityBar.h"
#include "BuildingMenu.h"
#include "ClickedInfo.h"
#include "GuardianInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Settlement/Tribe.h"
#include "GOTA/UI/Menu/IngameMenu.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/UI/DebugMenu/DebugMenu.h"

// ------------------------------- LifeCycle -------------------------------

void UIngameUI::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->OnGameEnding.AddDynamic(this, &UIngameUI::OnGameEnding);
	GameState->OnGuardiansChanged.AddDynamic(this, &UIngameUI::RefreshGuardianWidgets);
	RefreshGuardianWidgets();
	BTN_Build->OnPressed.AddDynamic(this, &UIngameUI::ToggleBuildMenu);
	BTN_Attack->OnPressed.AddDynamic(this, &UIngameUI::SetAllNativeArmiesToAttack);
}

// ------------------------------- Utility -------------------------------

void UIngameUI::HoverActor(AActor* Actor)
{
}

void UIngameUI::HandleEscapePressed()
{
	ToggleMenu();
}

// ------------------------------- Ingame Menu -------------------------------

void UIngameUI::ToggleMenu()
{
	IngameMenu->Toggle();
}

// ------------------------------- Guardian Info -------------------------------

void UIngameUI::RefreshGuardianWidgets()
{
	if (!GameState) return;
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

void UIngameUI::ToggleDebugMenu()
{
	if (DebugMenu->GetVisibility() == ESlateVisibility::Hidden)
	{
		DebugMenu->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DebugMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

// ------------------------------- Attack -------------------------------

void UIngameUI::SetAllNativeArmiesToAttack()
{
	ASettlement* NativeSettlement = GameState->GetTribe();
	if (NativeSettlement)
	{
		NativeSettlement->SetAllArmiesOnAttack();
	}
}
