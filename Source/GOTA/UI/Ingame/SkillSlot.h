#pragma once

#include "Blueprint/UserWidget.h"
#include "SkillSlot.generated.h"

class UImage;

UCLASS(Blueprintable)
class GOTA_API USkillSlot : public UUserWidget
{
	GENERATED_BODY()
	
	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Skill_Disk;

	// --------------------------------------------------
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void SetSkill(UTexture2D* Skill);
};
