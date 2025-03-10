#include "AbilitySlot.h"

#include "GOTA/Guardian/AbilitySlotRegister.h"
#include "GOTA/Guardian/Ability.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "GOTA/Guardian/AbilitySettings.h"

// ---------------------------------------- Lifecycle ----------------------------------------

void UAbilitySlot::Init(FName InSlotName)
{
	SlotName = InSlotName;
	if (UAbilitySlotRegister* AbilitySlotRegister = GetGameInstance()->GetSubsystem<UAbilitySlotRegister>())
	{
		AbilitySlotRegister->RegisterAbilitySlotWidget(SlotName, this);
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
	UAbilitySlotRegister* AbilitySlotRegister = GetGameInstance()->GetSubsystem<UAbilitySlotRegister>();
	if (AbilitySlotRegister)
	{
		AbilitySlotRegister->SwapAbilitiesInSlots(OriginSlot->GetSlotName(), SlotName);
	}
}
