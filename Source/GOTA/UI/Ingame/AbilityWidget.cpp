#include "AbilityWidget.h"

#include "Components/Image.h"
#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "GOTA/CoreSystems/Guardian/AbilitySettings.h"

void UAbilityWidget::Init(AAbility* InAbility)
{
	Ability = InAbility;
	Image->SetBrushFromTexture(Ability->GetSettings()->GetIcon());
}
