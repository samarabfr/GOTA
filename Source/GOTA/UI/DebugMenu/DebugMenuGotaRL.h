#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenuGotaRL.generated.h"

class ISnapshotAgent;
class UComboBoxString;
class UEditableTextBox;
class UButton;

UCLASS()
class GOTA_API UDebugMenuGotaRL : public UUserWidget
{
	GENERATED_BODY()

	// -------------------------------------------- LifeCycle --------------------------------------------
protected:
	virtual void NativeConstruct() override;

	// -------------------------------------------- Utility --------------------------------------------
private:

	// -------------------------------------------- Snapshots --------------------------------------------
private:
	TArray<TScriptInterface<ISnapshotAgent>> SnapshotAgents;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_AgentSelection;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TB_ModelName;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_SaveModel;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_LoadModel;

	UFUNCTION()
	void SaveModel();

	UFUNCTION()
	void LoadModel();

	TScriptInterface<ISnapshotAgent> GetAgentByName(const FString& AgentName);
	TArray<FString> GetAllAgentNames();
};
