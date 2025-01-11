#include "AbilitySlot.h"

#include "GOTA/CoreSystems/Guardian/AbilityManager.h"
#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "GOTA/CoreSystems/Guardian/AbilitySettings.h"

// ---------------------------------------- Lifecycle ----------------------------------------

void UAbilitySlot::Init(FName InSlotName)
{
	SlotName = InSlotName;
	if (UAbilityManager* AbilityManager = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>())
	{
		AbilityManager->RegisterAbilitySlot(SlotName, this);
	}
}

// ---------------------------------------- Utility ----------------------------------------

// ---------------------------------------- Ability ----------------------------------------

AAbility* UAbilitySlot::GetAbility() const
{
	return Ability.Get();
}

void UAbilitySlot::SetAbility(AAbility* NewAbility)
{
	Ability = NewAbility;
	if (NewAbility != nullptr)
	{
		AbilityImage->SetBrushFromTexture(Ability->GetSettings()->GetIcon());
		AbilityImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AbilityImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

// ---------------------------------------- SlotName ----------------------------------------

// ---------------------------------------- Drag & Drop ----------------------------------------

FReply UAbilitySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (Ability != nullptr)
	{
		// Start Dragging
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else
	{
		return FReply::Handled();
	}
}


void UAbilitySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                        UDragDropOperation*& OutOperation)
{
	UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
	DragDropOp->Payload = this;

	UImage* VisualDrag = NewObject<UImage>();
	VisualDrag->SetBrush(AbilityImage->GetBrush());
	DragDropOp->DefaultDragVisual = VisualDrag;

	OutOperation = DragDropOp;
}


bool UAbilitySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                UDragDropOperation* InOperation)
{
	// If this Slot has an Ability, reject the DragDrop
	if (Ability != nullptr) return false;

	if (InOperation->Payload && InOperation->Payload->IsA(StaticClass()))
	{
		// Dragging from another AbilitySlot into this one
		UAbilitySlot* OriginSlot = Cast<UAbilitySlot>(InOperation->Payload);
		if (OriginSlot != nullptr)
		{
			OnSuccessfulDrop(OriginSlot);
			return true;
		}
	}
	return false;
}

void UAbilitySlot::OnSuccessfulDrop(UAbilitySlot* OriginSlot)
{
	SetAbility(OriginSlot->GetAbility());
	OriginSlot->SetAbility(nullptr);
}
