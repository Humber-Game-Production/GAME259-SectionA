// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME259AGameMode.h"
#include "GAME259ACharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

AGAME259AGameMode::AGAME259AGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Core/Characters/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//make sure GM can tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);

	timerTime = 5.f;
	maxRounds = 5;
	currentRound = maxRounds;
}

void AGAME259AGameMode::BeginPlay() {
	Super::BeginPlay();
	
	//create timer
	GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, timerTime);
}

void AGAME259AGameMode::Tick(const float deltaTime) {
	Super::Tick(deltaTime);

	//add timer to screen
	if (GetWorldTimerManager().TimerExists(timerHandle) && GetWorldTimerManager().IsTimerPaused(timerHandle) == false) {
		timeLeft = FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(timerHandle));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, timeLeft);
	}
}

//Resets all the actors in the rounds
//Current only debug messages and a timer reset
void AGAME259AGameMode::Reset() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Flags respawn."));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Players respawn."));
	GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, timerTime);
}

void AGAME259AGameMode::EndRound() {
	//Displays the round that just finished
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Round " + FString::FromInt(currentRound) + " over" );

	//Reduces the amount of rounds left
	currentRound--;

	//Prints out how many rounds are left
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(currentRound) + " rounds remaining");

	//Checks to see if a win condition is met
	if (currentRound == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game ends."));
	}
	else
	{
		//Intermission? (pause)

		Reset();
	}

}

void AGAME259AGameMode::SubtractTime() {
	if (GetWorldTimerManager().TimerExists(timerHandle)) {
		GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, GetWorldTimerManager().GetTimerRemaining(timerHandle) - 1);
	}
}
