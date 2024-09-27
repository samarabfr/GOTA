#pragma once

#include "Blueprint/UserWidget.h"
#include "ClickedInfo.generated.h"

class UTileInfo;

UCLASS(Blueprintable)
class GOTA_API UClickedInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	// -------------------Widgets------------------------
	
	UPROPERTY(meta = (BindWidget))
	UTileInfo* TileInfo;
	
	// --------------------------------------------------
	
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void WatchActor(AActor* Actor);
};
