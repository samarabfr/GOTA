#include "IngameUI.h"

#include "BuildingMenu.h"
#include "ClickedInfo.h"
#include "GuardianInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

void UIngameUI::NativeConstruct()
{
	Super::NativeConstruct();
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->OnGameEnding.AddDynamic(this, &UIngameUI::OnGameEnding);
	GameState->OnGuardiansChanged.AddDynamic(this, &UIngameUI::RefreshGuardianWidgets);
	RefreshGuardianWidgets(GameState);
	Btn_Build->OnPressed.AddDynamic(this, &UIngameUI::OnBtnBuildPressed);
}

void UIngameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UIngameUI::RefreshGuardianWidgets(AGS_Ingame* GameState)
{
	GuardianInfo1->SetGuardian(GameState->GetGuardian(0));
	GuardianInfo2->SetGuardian(GameState->GetGuardian(1));
	GuardianInfo3->SetGuardian(GameState->GetGuardian(2));
	GuardianInfo4->SetGuardian(GameState->GetGuardian(3));
}

void UIngameUI::ClickActor(AActor* Actor)
{
	BuildingMenu->SetVisibility(ESlateVisibility::Hidden);
	ClickedInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ClickedInfo->WatchActor(Actor);
}

void UIngameUI::HoverActor(AActor* Actor)
{
}

void UIngameUI::WatchCombat(ACombat* Combat)
{
	// TODO: implement
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UIngameUI::OnGameEnding(const EGameEnding Ending, const FString& EndingMessage)
{
	Txt_GameEnding->SetText(FText::FromString(EndingMessage));
}

void UIngameUI::OnBtnBuildPressed()
{
	BuildingMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ClickedInfo->SetVisibility(ESlateVisibility::Hidden);
}
