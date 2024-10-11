#include "GuardianInfo.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Guardian/GuardianSettings.h"

void UGuardianInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGuardianInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGuardianInfo::SetGuardian(AGuardian* NewGuardian)
{
	Guardian = NewGuardian;
	if (!Guardian) return;
	if (const UGuardianSettings* GuardianSettings = Guardian->GetSettings())
	{
		Icon->SetBrushFromTexture(GuardianSettings->Icon);
		Name->SetText(FText::FromString(GuardianSettings->Name));
	}
}
