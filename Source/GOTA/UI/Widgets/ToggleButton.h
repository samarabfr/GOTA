#pragma once

#include "Blueprint/UserWidget.h"
#include "ToggleButton.generated.h"

UCLASS()
class GOTA_API UToggleButton : public UUserWidget
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------
private:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// ------------------- Utility -------------------
private:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveChangedSig, bool, NewActiveState);

public:
	bool IsActive() const { return bIsActive; }

	void SetIsActive(bool NewActiveState, bool SkipAnimation);
	
	void ToggleIsActive(bool SkipAnimation = false);

	void Activate(bool SkipAnimation = false);

	void Deactivate(bool SkipAnimation = false);

	FOnActiveChangedSig OnActiveChanged;

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim ))
	UWidgetAnimation* ToggleAnim;

private:
	bool bIsActive = false;
};
