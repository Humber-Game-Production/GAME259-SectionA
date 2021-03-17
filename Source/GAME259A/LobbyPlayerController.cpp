// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

ALobbyPlayerController::ALobbyPlayerController()
{
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Make sure only the Client Version of this PlayerController calls the ServerRPC
	/*if (Role < ROLE_Authority)
		Server_IncreaseVariable();*/

	// Assuming you are not already in the PlayerController (if you are, just call ClientTravel directly)
	/*APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->ClientTravel(“IPADDRESS”, ETravelType::TRAVEL_Absolute);*/


}

void ALobbyPlayerController::IncreaseVariable()
{
}

//bool ALobbyPlayerController::Server_IncreaseVariable() {
//	return true;
//}

//void ALobbyPlayerController::Server_IncreaseVariable_Implementation() {
//	ATestGameState* GameState = Cast<ATestGameState>(UGameplayStatics::GetGameState(GetWorld()));
//	GameState->IncreaseVariable();
//}

