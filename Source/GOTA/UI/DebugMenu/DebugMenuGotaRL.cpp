#include "DebugMenuGotaRL.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

// -------------------------------------------- LifeCycle --------------------------------------------

void UDebugMenuGotaRL::NativeConstruct()
{
	Super::NativeConstruct();
	/*
	if (AGS_GotaRL_Ingame* GameState = GetWorld()->GetGameState<AGS_GotaRL_Ingame>())
	{
		SimulatedGuardianManager = GameState->GetLearningManager();
	}
	*/
	BTN_SaveModel->OnClicked.AddDynamic(this, &UDebugMenuGotaRL::SaveModel);
	BTN_LoadModel->OnClicked.AddDynamic(this, &UDebugMenuGotaRL::LoadModel);
}

// -------------------------------------------- Utility --------------------------------------------

// -------------------------------------------- Snapshots --------------------------------------------

void UDebugMenuGotaRL::SaveModel()
{
	//SimulatedGuardianManager->SaveModel(SnapshotsFolderFilePath, TB_ModelName->GetText().ToString());
}

void UDebugMenuGotaRL::LoadModel()
{
	//SimulatedGuardianManager->LoadModel(SnapshotsFolderFilePath, TB_ModelName->GetText().ToString());
}
