#pragma once

#include "Blueprint/UserWidget.h"
#include "LoadingProgress.generated.h"

class ALoadingStatusActor;
class UTextBlock;

UCLASS(Blueprintable)
class GOTA_API ULoadingProgress : public UUserWidget
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------

public:
	void Init(ALoadingStatusActor* InLoadingStatus);

private:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// ------------------- Utility -------------------

	UPROPERTY()
	ALoadingStatusActor* LoadingStatus;

public:
	bool IsInitialized() const { return LoadingStatus != nullptr; }
	
	// ------------------- Widgets -------------------
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerID;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Status;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReplicationCount;
};
