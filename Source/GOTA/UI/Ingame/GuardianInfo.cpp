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
	Icon->SetBrushFromTexture(Guardian->Settings->Icon);
	Name->SetText(FText::FromString(Guardian->Settings->Name));
}
