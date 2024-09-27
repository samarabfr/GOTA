#include "IngameUI.h"

#include "ClickedInfo.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

void UIngameUI::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetGameState<AGS_Ingame>()->OnGameEnding.AddDynamic(this, &UIngameUI::OnGameEnding);
}

void UIngameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UIngameUI::ClickActor(AActor* Actor)
{
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
