#pragma once

#include "Blueprint/UserWidget.h"
#include "BuildingMenuSlot.generated.h"

class UImage;
class UTextBlock;
class UBuildingSettings;

UCLASS(Blueprintable)
class GOTA_API UBuildingMenuSlot : public UUserWidget
{
	GENERATED_BODY()

	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Icon;

	// --------------------------------------------------
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY()
	UBuildingSettings* BuildingSettings;

public:
	void SetBuildingSettings(UBuildingSettings* NewBuildingSettings);
	UBuildingSettings* GetBuildingSettings() { return BuildingSettings; }
};
