#pragma once

#include "Blueprint/UserWidget.h"
#include "GuardianInfo.generated.h"


class UTextBlock;
class AGuardian;
class UImage;

UCLASS(Blueprintable)
class GOTA_API UGuardianInfo : public UUserWidget
{
	GENERATED_BODY()


	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;

	// --------------------------------------------------

private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	AGuardian* Guardian;

public:
	void SetGuardian(AGuardian* NewGuardian);
};
