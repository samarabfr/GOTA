#pragma once

#include "Blueprint/UserWidget.h"
#include "MultiplayerMenu.generated.h"

class UButton;
class UEditableTextBox;

UCLASS()
class GOTA_API UMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------
private:
	virtual void NativeConstruct() override;

	// ------------------- Utility -------------------
public:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Back;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TB_IpAddress;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_OpenLobby;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_JoinWithIP;

private:
	UFUNCTION()
	void OpenLobby();
	
	UFUNCTION()
	void JoinWithIp();

	void DisableInput();
};
