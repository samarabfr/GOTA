#include "DebugMenuGotaRL.h"

#include "LearningAgentsNeuralNetwork.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "GOTA/ReinforcementLearning/SnapshotSystem/SnapshotAgentData.h"
#include "GOTA/ReinforcementLearning/SnapshotSystem/SnapshotAgents.h"

// -------------------------------------------- LifeCycle --------------------------------------------

void UDebugMenuGotaRL::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_SaveModel->OnClicked.AddDynamic(this, &UDebugMenuGotaRL::SaveModel);
	BTN_LoadModel->OnClicked.AddDynamic(this, &UDebugMenuGotaRL::LoadModel);
	CB_AgentSelection->OnSelectionChanged.AddDynamic(this, &UDebugMenuGotaRL::RefreshNeuralNetworkOptions);
	if (SnapshotAgents == nullptr) return;
	for (FString AgentName : SnapshotAgents->GetAllAgentNames())
	{
		CB_AgentSelection->AddOption(AgentName);
	}
	CB_AgentSelection->SetSelectedOption(CB_AgentSelection->GetOptionAtIndex(0));
}

// -------------------------------------------- Utility --------------------------------------------

// -------------------------------------------- Snapshots --------------------------------------------

void UDebugMenuGotaRL::SaveModel()
{
	if (SnapshotAgents == nullptr) return;
	USnapshotAgentData* Agent = SnapshotAgents->GetAgentByName(CB_AgentSelection->GetSelectedOption());
	if (Agent == nullptr) return;
	const FString ModelName = TB_ModelName->GetText().ToString();
	const FString NeuralNetworkName = CB_NeuralNetworkSelection->GetSelectedOption();
	const FSnapshotNeuralNetworkData NeuralNetwork = Agent->GetNeuralNetworkByName(NeuralNetworkName);
	if (NeuralNetwork.Name == "Unnamed") return;
	FFilePath ModelPath;
	ModelPath.FilePath = FPaths::ProjectContentDir() /
		SnapshotAgents->GetSnapshotsFolderFilePath().FilePath /
		ModelName;
	FFilePath FullSnapshotPath;
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Critic";
	NeuralNetwork.Critic->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Encoder";
	NeuralNetwork.Encoder->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Policy";
	NeuralNetwork.Policy->SaveNetworkToSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Decoder";
	NeuralNetwork.Decoder->SaveNetworkToSnapshot(FullSnapshotPath);
}

void UDebugMenuGotaRL::LoadModel()
{
	if (SnapshotAgents == nullptr) return;
	USnapshotAgentData* Agent = SnapshotAgents->GetAgentByName(CB_AgentSelection->GetSelectedOption());
	if (Agent == nullptr) return;
	const FString ModelName = TB_ModelName->GetText().ToString();
	const FString NeuralNetworkName = CB_NeuralNetworkSelection->GetSelectedOption();
	const FSnapshotNeuralNetworkData NeuralNetwork = Agent->GetNeuralNetworkByName(NeuralNetworkName);
	if (NeuralNetwork.Name == "Unnamed") return;
	FFilePath ModelPath;
	ModelPath.FilePath = FPaths::ProjectContentDir() /
		SnapshotAgents->GetSnapshotsFolderFilePath().FilePath /
		ModelName;
	FFilePath FullSnapshotPath;
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Critic";
	NeuralNetwork.Critic->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Encoder";
	NeuralNetwork.Encoder->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Policy";
	NeuralNetwork.Policy->LoadNetworkFromSnapshot(FullSnapshotPath);
	FullSnapshotPath.FilePath = ModelPath.FilePath + "Decoder";
	NeuralNetwork.Decoder->LoadNetworkFromSnapshot(FullSnapshotPath);
}

void UDebugMenuGotaRL::RefreshNeuralNetworkOptions(FString AgentName, ESelectInfo::Type SelectInfo)
{
	USnapshotAgentData* Agent = SnapshotAgents->GetAgentByName(AgentName);
	if (Agent == nullptr) return;
	for (FString NeuralNetworkName : Agent->GetAllNeuralNetworkNames())
	{
		CB_NeuralNetworkSelection->AddOption(NeuralNetworkName);
	}
	CB_NeuralNetworkSelection->SetSelectedOption(CB_NeuralNetworkSelection->GetOptionAtIndex(0));
}
