#include "LobbyPlayer.h"

#include "Net/UnrealNetwork.h"

void ULobbyPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ULobbyPlayer, SelectedGuardian);
}

bool ULobbyPlayer::IsSupportedForNetworking() const
{
	return true;
}

void ULobbyPlayer::OnRep_SelectedGuardian()
{
	OnSelectedGuardianChanged.Broadcast(SelectedGuardian);
}

void ULobbyPlayer::OnRep_PlayerState()
{
	OnPlayerStateChanged.Broadcast(PlayerState);
}
