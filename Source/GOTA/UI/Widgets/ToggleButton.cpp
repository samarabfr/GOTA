#include "ToggleButton.h"

#include "Animation/WidgetAnimation.h"

void UToggleButton::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UToggleButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ToggleIsActive();
	return FReply::Handled();
}

FReply UToggleButton::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ToggleIsActive();
	return FReply::Handled();
}

void UToggleButton::SetIsActive(const bool NewActiveState, const bool SkipAnimation)
{
	if (NewActiveState)
		Activate(SkipAnimation);
	else
		Deactivate(SkipAnimation);
}

void UToggleButton::ToggleIsActive(const bool SkipAnimation)
{
	if (bIsActive)
		Deactivate(SkipAnimation);
	else
		Activate(SkipAnimation);
}

void UToggleButton::Activate(const bool SkipAnimation)
{
	if (bIsActive) return;
	const float StartTime = SkipAnimation ? ToggleAnim->GetEndTime() : 0;
	PlayAnimation(ToggleAnim, StartTime);
	bIsActive = true;
	OnActiveChanged.Broadcast(true);
}

void UToggleButton::Deactivate(const bool SkipAnimation)
{
	if (!bIsActive) return;
	const float StartTime = SkipAnimation ? ToggleAnim->GetEndTime() : 0;
	PlayAnimation(ToggleAnim, StartTime, 1, EUMGSequencePlayMode::Reverse);
	bIsActive = false;
	OnActiveChanged.Broadcast(false);
}
