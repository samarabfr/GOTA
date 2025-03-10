#include "MultiplayerMenu.h"

#include "GM_MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UMultiplayerMenu::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_OpenLobby->OnPressed.AddDynamic(this, &UMultiplayerMenu::OpenLobby);
	BTN_JoinWithIP->OnPressed.AddDynamic(this, &UMultiplayerMenu::JoinWithIp);
}

void UMultiplayerMenu::OpenLobby()
{
	DisableInput();
	AGM_MainMenu* GameMode = GetWorld()->GetAuthGameMode<AGM_MainMenu>();
	if (GameMode)
		GameMode->StartGame(true);
}

void UMultiplayerMenu::JoinWithIp()
{
	DisableInput();

	AGM_MainMenu* GameMode = GetWorld()->GetAuthGameMode<AGM_MainMenu>();
	if (GameMode)
		GameMode->JoinGame(	TB_IpAddress->GetText().ToString());
}

void UMultiplayerMenu::DisableInput()
{
	BTN_OpenLobby->SetIsEnabled(false);
	BTN_JoinWithIP->SetIsEnabled(false);
	TB_IpAddress->SetIsEnabled(false);
	BTN_Back->SetIsEnabled(false);
}
