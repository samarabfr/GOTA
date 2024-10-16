#include "ClickBlocker.h"

FReply UClickBlocker::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UClickBlocker::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}
