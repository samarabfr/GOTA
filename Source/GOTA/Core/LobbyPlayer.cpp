#include "LobbyPlayer.h"

#include "Net/UnrealNetwork.h"

void ULobbyPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ULobbyPlayer, SelectedGuardian);
	DOREPLIFETIME(ULobbyPlayer, PlayerState);
}

bool ULobbyPlayer::IsSupportedForNetworking() const
{
	return true;
}

void ULobbyPlayer::SetSelectedGuardian(TSubclassOf<AGuardian> NewSelectedGuardian)
{
	SelectedGuardian = NewSelectedGuardian;
	OnSelectedGuardianChanged.Broadcast(SelectedGuardian);
}

void ULobbyPlayer::OnRep_SelectedGuardian()
{
	OnSelectedGuardianChanged.Broadcast(SelectedGuardian);
}

void ULobbyPlayer::SetPlayerState(APlayerState* NewPlayerState)
{
	PlayerState = NewPlayerState;
	OnPlayerStateChanged.Broadcast(PlayerState);
}

void ULobbyPlayer::OnRep_PlayerState()
{
	OnPlayerStateChanged.Broadcast(PlayerState);
}
