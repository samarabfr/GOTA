#pragma once

#include "Components/Button.h"
#include "Options.generated.h"

UCLASS()
class GOTA_API UOptions : public UUserWidget
{
	GENERATED_BODY()
	
	// ------------------- LifeCycle -------------------
private:
	virtual void NativeConstruct() override;

	// ------------------- Utility -------------------

	
	// ------------------- Widgets -------------------
public:
		
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Start;


	
};
