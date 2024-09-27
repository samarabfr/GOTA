#include "IngameUI.h"

#include "ClickedInfo.h"

void UIngameUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIngameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UIngameUI::ClickActor(AActor* Actor)
{
	ClickedInfo->WatchActor(Actor);
}
