#include "MainMenu.h"

#include "MultiplayerMenu.h"
#include "Options.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GOTA/Content/MainMenu/GM_MainMenu.h"
#include "Kismet/KismetSystemLibrary.h"

// ------------------- LifeCycle -------------------

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_SinglePlayer->OnPressed.AddDynamic(this, &UMainMenu::StartSinglePlayer);
	BTN_Multiplayer->OnPressed.AddDynamic(this, &UMainMenu::OpenMultiplayerMenu);
	BTN_Settings->OnPressed.AddDynamic(this, &UMainMenu::OpenSettings);
	BTN_Quit->OnPressed.AddDynamic(this, &UMainMenu::QuitGame);

	WBP_Options->BTN_Back->OnPressed.AddDynamic(this, &UMainMenu::OpenMainMenu);
	WBP_MultiplayerMenu->BTN_Back->OnPressed.AddDynamic(this, &UMainMenu::OpenMainMenu);
}

// ------------------- Utility -------------------

void UMainMenu::StartSinglePlayer()
{
	AGM_MainMenu* GameMode = GetWorld()->GetAuthGameMode<AGM_MainMenu>();
	GameMode->StartGame(false);
}

void UMainMenu::OpenMultiplayerMenu()
{
	VB_Menu->SetVisibility(ESlateVisibility::Hidden);
	WBP_MultiplayerMenu->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenu::OpenSettings()
{
	VB_Menu->SetVisibility(ESlateVisibility::Hidden);
	WBP_Options->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenu::OpenMainMenu()
{
	VB_Menu->SetVisibility(ESlateVisibility::Visible);
	WBP_Options->SetVisibility(ESlateVisibility::Hidden);
	WBP_MultiplayerMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}


