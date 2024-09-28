#pragma once

#include "Blueprint/UserWidget.h"
#include "SkillBar.generated.h"

class USkillSlot;

UCLASS(Blueprintable)
class GOTA_API USkillBar : public UUserWidget
{
	GENERATED_BODY()
	
	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot1;

	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot2;

	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot3;
	
	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot4;

	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot5;

	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot6;
	
	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot7;

	UPROPERTY(meta = (BindWidget))
	USkillSlot* SkillSlot8;

	// --------------------------------------------------
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TArray<USkillSlot*> SkillSlots;

protected:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* TestSkillTexture;
};
