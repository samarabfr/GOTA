#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenuGotaRL.generated.h"

class USnapshotAgentData;
class UComboBoxString;
class USnapshotAgents;
class UEditableTextBox;
class ASimulatedGuardianManager;
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
	TWeakObjectPtr<ASimulatedGuardianManager> SimulatedGuardianManager;

	// -------------------------------------------- Snapshots --------------------------------------------

	UPROPERTY(EditAnywhere)
	USnapshotAgents* SnapshotAgents;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_AgentSelection;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CB_NeuralNetworkSelection;

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

	UFUNCTION()
	void RefreshNeuralNetworkOptions(FString AgentName, ESelectInfo::Type SelectInfo);
};
