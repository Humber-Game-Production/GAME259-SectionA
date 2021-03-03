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
}

void ALobbyGameMode::Tick(const float deltaTime) {
	Super::Tick(deltaTime);
}

void ALobbyGameMode::ServerUpdateGameOptions(int MapID_, int TimeID_) {
	MapID = MapID_;
	TimeID = TimeID_;
}