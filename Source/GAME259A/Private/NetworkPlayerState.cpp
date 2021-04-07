// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"
#include <Runtime\Engine\Public\Net\UnrealNetwork.h>
#include <GAME259A\Public\NetworkGameInstance.h>

ANetworkPlayerState::ANetworkPlayerState()
{
	bIsReady = false;
}

void ANetworkPlayerState::BeginPlay()
{

	Super::BeginPlay();

	//change the player name to the name in the game insance if it is on lan
	ChangePlayerName();

}

void ANetworkPlayerState::ChangePlayerName()
{
	//check if this has authority
	if (HasAuthority())
	{
		//try to get the NetworkedGameInstance
		UNetworkGameInstance* NetworkGameInstance = Cast<UNetworkGameInstance>(GetWorld()->GetGameInstance());

		//if the game instance is not null, get the player name from it
		if (NetworkGameInstance)
		{
			FString ActualPlayerName = NetworkGameInstance->GetPlayerName();
			//if the string is empty that means we are on steam so no need to change the name
			if (ActualPlayerName != "")
				//if we are on lan set the player name to the name we got from game instance
				SetPlayerName(ActualPlayerName);
		}
	}
	else //if the player doesn't have authority call the serverside to call this function again
		Server_ChangePlayerName();
}

void ANetworkPlayerState::Server_ChangePlayerName_Implementation()
{
	//call the change player name fucntion from server side
	ChangePlayerName();
}


void ANetworkPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetworkPlayerState, bIsReady);
}