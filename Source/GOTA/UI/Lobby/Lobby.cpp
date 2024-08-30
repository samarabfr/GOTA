#include "Lobby.h"
#include "GOTA/CoreSystems/GameplayFramework/GM_Ingame.h"

void ULobby::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	BTN_Start->OnPressed.AddDynamic(this, &ULobby::StartPressed);
	BTN_Leave->OnPressed.AddDynamic(this, &ULobby::LeavePressed);
	if (GetWorld()->GetNetMode() == NM_Client)
	{
		// Client Construct
		BTN_Start->SetIsEnabled(false);
		SB_IslandSize->SetIsEnabled(false);
		SB_Colonies->SetIsEnabled(false);
	}
	else
	{
		// Server Construct
		BTN_Start->SetIsEnabled(true);

		SB_IslandSize->SetIsEnabled(true);
		SB_IslandSize->OnValueCommitted.AddDynamic(this, &ULobby::IslandTilesChanged);
		IslandTilesChanged(SB_IslandSize->GetValue(), ETextCommit::Default);

		SB_Colonies->SetIsEnabled(true);
		SB_Colonies->OnValueCommitted.AddDynamic(this, &ULobby::ColonyCountChanged);
		ColonyCountChanged(SB_Colonies->GetValue(), ETextCommit::Default);
	}
	GameState->StartParameter->OnChanged.AddDynamic(this, &ULobby::StartParameterChanged);
	StartParameterChanged(GameState->StartParameter);
	PlayerSlots.Add(PlayerSlot1);
	PlayerSlots.Add(PlayerSlot2);
	PlayerSlots.Add(PlayerSlot3);
	PlayerSlots.Add(PlayerSlot4);
}

void ULobby::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TArray<bool> StillConnected;
	StillConnected.SetNumZeroed(PlayerSlots.Num());
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		APS_Ingame* PS = Cast<APS_Ingame>(PlayerState);
		if (!PS || !PlayerSlots.IsValidIndex(PS->GOTAPlayerID))
			continue;
		StillConnected[PS->GOTAPlayerID] = true;
		if (PlayerSlots[PS->GOTAPlayerID]->CachedPlayerState != PS)
		{
			PlayerSlots[PS->GOTAPlayerID]->SetPlayerState(PS);
		}
	}
	for (int32 i = 0; i < PlayerSlots.Num(); ++i)
	{
		if (!StillConnected[i])PlayerSlots[i]->SetPlayerState(nullptr);
	}
}

void ULobby::StartPressed()
{
	// just to make sure that clients can't call this
	if (GetWorld()->GetNetMode() == NM_Client) return;
	BTN_Start->SetIsEnabled(false);
	GetWorld()->GetAuthGameMode<AGM_Ingame>()->LoadGame();
}


void ULobby::LeavePressed()
{
	GetWorld()->GetFirstPlayerController()->ClientTravel(
		"/Game/UI/MainMenu/L_MainMenu?listen",
		TRAVEL_Absolute);
}

void ULobby::ColonyCountChanged(float InValue, ETextCommit::Type CommitMethod)
{
	GameState->StartParameter->SetColonies(InValue);
}

void ULobby::IslandTilesChanged(float InValue, ETextCommit::Type CommitMethod)
{
	GameState->StartParameter->SetIslandSize(InValue);
}

void ULobby::StartParameterChanged(UStartParameter* StartParameter)
{
	SB_IslandSize->SetValue(StartParameter->GetIslandSize());
	SB_Colonies->SetValue(StartParameter->GetColonies());
}
