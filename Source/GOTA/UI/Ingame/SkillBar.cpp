#include "SkillBar.h"

#include "SkillSlot.h"

void USkillBar::NativeConstruct()
{
	Super::NativeConstruct();
	SkillSlots.Add(SkillSlot1);
	SkillSlots.Add(SkillSlot2);
	SkillSlots.Add(SkillSlot3);
	SkillSlots.Add(SkillSlot4);
	SkillSlots.Add(SkillSlot5);
	SkillSlots.Add(SkillSlot6);
	SkillSlots.Add(SkillSlot7);
	SkillSlots.Add(SkillSlot8);

	SkillSlot1->SetSkill(TestSkillTexture);
	SkillSlot2->SetSkill(TestSkillTexture);
	SkillSlot3->SetSkill(TestSkillTexture);
	SkillSlot5->SetSkill(TestSkillTexture);
}

void USkillBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

