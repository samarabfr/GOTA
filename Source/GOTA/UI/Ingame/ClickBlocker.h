#pragma once

#include "Blueprint/UserWidget.h"
#include "ClickBlocker.generated.h"

class AMouseUtils;

UCLASS(Blueprintable)
class GOTA_API UClickBlocker : public UUserWidget
{
	GENERATED_BODY()

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
