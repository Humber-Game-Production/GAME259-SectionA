// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode() {
	MapID = 0;
	TimeID = 0;
}

void ALobbyGameMode::BeginPlay() {
	Super::BeginPlay();

	//create timer
	//GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, timerTime);
}

void ALobbyGameMode::Tick(const float deltaTime) {
	Super::Tick(deltaTime);

	//add timer to screen
	//if (GetWorldTimerManager().TimerExists(timerHandle) && GetWorldTimerManager().IsTimerPaused(timerHandle) == false) {
	//	timeLeft = FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(timerHandle));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, timeLeft);
	//}
}

void ALobbyGameMode::ServerUpdateGameOptions(int MapID_, int TimeID_) {
	MapID = MapID_;
	TimeID = TimeID_;
}