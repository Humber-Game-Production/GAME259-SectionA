// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameMode.h"
#include "../GAME259ACharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

ACTFGameMode::ACTFGameMode()
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

	team1Points = 26;
	team2Points = 16;
	timerTime = 5.f;
	maxRounds = 5;
	currentRound = maxRounds;
}

void ACTFGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Create timer
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACTFGameMode::EndRound, timerTime);
}

void ACTFGameMode::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);

	//Add timer to screen
	if (GetWorldTimerManager().TimerExists(timerHandle) && GetWorldTimerManager().IsTimerPaused(timerHandle) == false) {
		timeLeft = FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(timerHandle));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, timeLeft);
	}
}

void ACTFGameMode::EndRound() {
	//Displays the round that just finished
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Round " + FString::FromInt(currentRound) + " over");

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
		if (WinCheck())
		{
			if (team1Points > team2Points)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Team1 wins"));
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Team2 wins"));
		}
		else
			RoundReset();
	}
}

//Resets all the actors in the rounds
//Current only debug messages and a timer reset
void ACTFGameMode::RoundReset() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Flags respawn."));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Players respawn."));
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACTFGameMode::EndRound, timerTime);
}


void ACTFGameMode::SubtractTime() {
	if (GetWorldTimerManager().TimerExists(timerHandle)) {
		GetWorldTimerManager().SetTimer(timerHandle, this, &ACTFGameMode::EndRound, GetWorldTimerManager().GetTimerRemaining(timerHandle) - 1);
	}
}

//Will return winning team later for now it is just checking to see if there is a winner
bool ACTFGameMode::WinCheck()
{
	switch (currentRound)
	{
	case(2): if (team1Points - team2Points > 18)
	{
		return true;
	}
		   else if (team2Points - team1Points > 18)
	{
		return true;
	}
		   else
		break;
	case(1): if (team1Points - team2Points > 9)
	{
		return true;
	}
		   else if (team2Points - team1Points > 9)
	{
		return true;
	}
		   else
		break;
	case(0): return true;
	default: break;
	}
	return false;
}