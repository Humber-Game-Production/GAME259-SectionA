// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode() {
	MapID = 0;
	TimeID = 0;
	lobbyGameMode = nullptr;
	MapNameDisplay = "Area 1";
	MapTimeDisplay = "10:00";
}

void ALobbyGameMode::BeginPlay() {
	Super::BeginPlay();

	// 'FCString::Atoi' converts 'FString' to 'int32' and we use the static 'ParseOption' function of the
// 'UGameplayStatics' Class to get the correct Key from the 'OptionsString'
	/*MaxNumPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "MaxNumPlayers")));*/
}

void ALobbyGameMode::Tick(const float deltaTime) {
	Super::Tick(deltaTime);
}

void ALobbyGameMode::ServerUpdateGameOptions(int MapID_, int TimeID_) {
	MapID = MapID_;
	TimeID = TimeID_;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	PlayerControllerList.Add(NewPlayer);
}