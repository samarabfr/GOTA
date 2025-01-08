#include "AbilitySlot.h"

#include "GOTA/CoreSystems/Guardian/AbilityManager.h"
#include "AbilityWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

// ---------------------------------------- Lifecycle ----------------------------------------

void UAbilitySlot::Init(FName InSlotName)
{
	SlotName = InSlotName;
	if (UAbilityManager* AbilityManager = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>())
	{
		AbilityManager->RegisterAbilitySlot(this, SlotName);
	}
}

// ---------------------------------------- Utility ----------------------------------------

AAbility* UAbilitySlot::GetAbility() const
{
	return AbilityWidget.IsValid() ? AbilityWidget->GetAbility() : nullptr;
}

void UAbilitySlot::SetAbilityWidget(UAbilityWidget* NewAbilityWidget)
{
	AbilityWidget = NewAbilityWidget;
	UCanvasPanelSlot* PanelSlot = Canvas->AddChildToCanvas(AbilityWidget.Get());
	PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	PanelSlot->SetPosition(FVector2D(0.0f, 0.0f));
	PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	PanelSlot->SetSize(FVector2D(64.0f, 64.0f));
}

void UAbilitySlot::SpawnAbilityWidget(AAbility* Ability)
{
	if (!Ability || !GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAbilityWidget: Invalid ability or world context."));
		return;
	}

	if (AbilityWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("SpawnAbilityWidget: AbilityWidget already exists for SlotName: %s."),
		       *SlotName.ToString());
		return;
	}

	UAbilityWidget* NewAbilityWidget = CreateWidget<UAbilityWidget>(GetWorld(), AbilityWidgetClass);
	if (!NewAbilityWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnAbilityWidget: Failed to create AbilityWidget."));
		return;
	}

	NewAbilityWidget->Init(Ability);
	NewAbilityWidget->AddToViewport();

	SetAbilityWidget(NewAbilityWidget);
}

// ---------------------------------------- SlotName ----------------------------------------