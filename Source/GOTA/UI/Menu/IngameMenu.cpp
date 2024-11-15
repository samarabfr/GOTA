#include "IngameMenu.h"

#include "Options.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"

void UIngameMenu::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Resume->OnPressed.AddDynamic(this, &UIngameMenu::Close);
	BTN_Settings->OnPressed.AddDynamic(this, &UIngameMenu::OpenOptions);
	BTN_Quit->OnPressed.AddDynamic(this, &UIngameMenu::QuitGame);

	WBP_Options->BTN_Back->OnPressed.AddDynamic(this, &UIngameMenu::CloseOptions);
}

void UIngameMenu::Toggle()
{
	if (IsAnimationPlaying(Anim_SwipeIn))
		return;
	
	if (CP_Container->IsVisible())
		Close();
	else
		Open();
}

void UIngameMenu::Open()
{
	PlayAnimation(Anim_SwipeIn);
	VB_Menu->SetVisibility(ESlateVisibility::Visible);
	WBP_Options->SetVisibility(ESlateVisibility::Hidden);
}

void UIngameMenu::Close()
{
	PlayAnimation(Anim_SwipeIn, 0, 1, EUMGSequencePlayMode::Reverse);
}

void UIngameMenu::OpenOptions()
{
	VB_Menu->SetVisibility(ESlateVisibility::Hidden);
	WBP_Options->SetVisibility(ESlateVisibility::Visible);
}

void UIngameMenu::CloseOptions()
{
	VB_Menu->SetVisibility(ESlateVisibility::Visible);
	WBP_Options->SetVisibility(ESlateVisibility::Hidden);
}

void UIngameMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
