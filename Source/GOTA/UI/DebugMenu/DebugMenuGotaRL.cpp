#include "DebugMenuGotaRL.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "GOTA/ReinforcementLearning/SnapshotSystem/SnapshotAgent.h"
#include "Kismet/GameplayStatics.h"

// -------------------------------------------- LifeCycle --------------------------------------------

void UDebugMenuGotaRL::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_SaveModel->OnClicked.AddDynamic(this, &UDebugMenuGotaRL::SaveModel);
	BTN_LoadModel->OnClicked.AddDynamic(this, &UDebugMenuGotaRL::LoadModel);
	// get all snapshot agents
	TArray<AActor*> SnapshotAgentsActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USnapshotAgent::StaticClass(), SnapshotAgentsActors);
	for (AActor* SnapshotAgentsActor : SnapshotAgentsActors)
	{
		if (SnapshotAgentsActor && SnapshotAgentsActor->Implements<USnapshotAgent>())
		{
			SnapshotAgents.Add(SnapshotAgentsActor);
		}
	}
	// snapshot agent selection
	for (FString AgentName : GetAllAgentNames())
	{
		CB_AgentSelection->AddOption(AgentName);
	}
	CB_AgentSelection->SetSelectedOption(CB_AgentSelection->GetOptionAtIndex(0));
}

// -------------------------------------------- Utility --------------------------------------------

// -------------------------------------------- Snapshots --------------------------------------------

void UDebugMenuGotaRL::SaveModel()
{
	TScriptInterface<ISnapshotAgent> Agent = GetAgentByName(CB_AgentSelection->GetSelectedOption());
	if (Agent == nullptr) return;
	const FString ModelName = TB_ModelName->GetText().ToString();
	Agent->SaveModel(ModelName);
}

void UDebugMenuGotaRL::LoadModel()
{
	if (SnapshotAgents.IsEmpty()) return;
	TScriptInterface<ISnapshotAgent> Agent = GetAgentByName(CB_AgentSelection->GetSelectedOption());
	if (Agent == nullptr) return;
	const FString ModelName = TB_ModelName->GetText().ToString();
	Agent->LoadModel(ModelName);
}

TScriptInterface<ISnapshotAgent> UDebugMenuGotaRL::GetAgentByName(const FString& AgentName)
{
	if (SnapshotAgents.IsEmpty()) return nullptr;
	for (TScriptInterface Agent : SnapshotAgents)
	{
		if (Agent->GetAgentName() == AgentName)
		{
			return Agent;
		}
	}
	return nullptr;
}

TArray<FString> UDebugMenuGotaRL::GetAllAgentNames()
{
	TArray<FString> AgentNames;
	for (TScriptInterface Agent : SnapshotAgents)
	{
		AgentNames.Add(Agent->GetAgentName());
	}
	return AgentNames;
}
