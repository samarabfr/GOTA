#pragma once

#include "Blueprint/UserWidget.h"
#include "AbilitySlot.generated.h"

class UCanvasPanel;
class UOverlay;
class AAbility;
class UAbilityImage;
class UImage;

UCLASS(Blueprintable)
class GOTA_API UAbilitySlot : public UUserWidget
{
	GENERATED_BODY()

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	void Init(FName InSlotName);

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay;

	UPROPERTY(meta = (BindWidget))
	UImage* AbilityImage;

	// ---------------------------------------- Ability ----------------------------------------
private:
	TWeakObjectPtr<AAbility> Ability;

public:
	void SetAbility(AAbility* NewAbility);

	AAbility* GetAbility() const;

	// ---------------------------------------- SlotName ----------------------------------------
private:
	FName SlotName;

public:
	FName GetSlotName() const { return SlotName; }

	// ---------------------------------------- Drag & Drop ----------------------------------------
private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;
};
