#pragma once

#include "Blueprint/UserWidget.h"
#include "TileInfo.generated.h"

class ATile;
class UProgressBar;
class UTextBlock;

UCLASS(Blueprintable)
class GOTA_API UTileInfo : public UUserWidget
{
	GENERATED_BODY()
	
	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tree_Current;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tree_Max;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tree_Growth;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Tree_Progress;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Wildlife_Current;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Wildlife_Max;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Wildlife_Growth;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Wildlife_Progress;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Forage_Current;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Forage_Max;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Forage_Growth;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Forage_Progress;

	// ---------------------------------------------------------
private:	
	UPROPERTY()
	ATile* CurrentTile;
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void WatchTile(ATile* Tile);
};
