#pragma once

#include "Blueprint/UserWidget.h"
#include "AbilityWidget.generated.h"

class AAbility;
class UImage;

UCLASS(Blueprintable)
class GOTA_API UAbilityWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image;

private:
	TWeakObjectPtr<AAbility> Ability;

public:
	void Init(AAbility* InAbility);
	AAbility* GetAbility() const { return Ability.Get(); };
};
