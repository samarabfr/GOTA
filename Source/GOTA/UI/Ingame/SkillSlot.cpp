#include "SkillSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"


void USkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USkillSlot::SetSkill(UTexture2D* Skill)
{
	if (Skill)
	{
		Skill_Disk->SetBrushFromTexture(Skill);
		Skill_Disk->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Skill_Disk->SetVisibility(ESlateVisibility::Hidden);
	}
}
