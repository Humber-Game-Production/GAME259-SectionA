// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameMode.h"
#include "../GAME259ACharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "GameFramework/HUD.h"
#include "GAME259A/GameMode/TeamIdentifier.h"

ACTFGameMode::ACTFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Game_BP/BaseCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> gameHudClass(TEXT("/Game/Game_BP/GameMode/BP_CTFHUD"));
	if(gameHudClass.Class != NULL)
	{
		HUDClass =  gameHudClass.Class; //gameHudClass.Class;
	}

	GameStateClass = ACTFGameState::StaticClass();
	PlayerStateClass = ACTFPlayerState::StaticClass();
	
	//make sure GM can tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	timerTime = 20.0f;
	maxRounds = 5;
	currentRound = 1;
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, timeLeft);
	}
}

void ACTFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	
}

void ACTFGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	Cast<ACTFGameState>(GameStateClass)->PlayerLeft(Exiting);
}

void ACTFGameMode::EndRound() {
	//Displays the round that just finished
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Round " + FString::FromInt(currentRound) + " over");
	
	//Reduces the amount of rounds left
	currentRound++;

	//Prints out how many rounds are left
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(currentRound) + " rounds remaining");

	//Checks to see if a win condition is met
	if (currentRound >= maxRounds) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game ends."));
	}
	else
	{
		//Intermission? (pause)
		if (WinCheck())
		{
			if (teamPoints[ETeamIdentifier::Human] > teamPoints[ETeamIdentifier::Alien])
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

	for (auto team : ctfGameState->listOfTeams)
	{
		team.Value->SpawnPlayers();
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Players respawn."));
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACTFGameMode::EndRound, timerTime);
}


//Will return winning team later for now it is just checking to see if there is a winner
bool ACTFGameMode::WinCheck()
{
	if(teamPoints.Num() != 0)
	{
		switch (currentRound)
		{
			case(2): 
			if (teamPoints[ETeamIdentifier::Human] - teamPoints[ETeamIdentifier::Alien] > 18)
			{
				UE_LOG(LogTemp, Warning, TEXT("Mercy Rule, Humans %d, Aliens %d"), teamPoints[ETeamIdentifier::Human], teamPoints[ETeamIdentifier::Alien]);
				return true;
			}
			else if (teamPoints[ETeamIdentifier::Alien] - teamPoints[ETeamIdentifier::Human] > 18)
			{
				UE_LOG(LogTemp, Warning, TEXT("Mercy Rule, Humans %d, Aliens %d"), teamPoints[ETeamIdentifier::Human], teamPoints[ETeamIdentifier::Alien]);
				return true;
			}
			break;
			case(1): 
			if (teamPoints[ETeamIdentifier::Human] - teamPoints[ETeamIdentifier::Alien] > 9)
			{
				UE_LOG(LogTemp, Warning, TEXT("Mercy Rule, Humans %d, Aliens %d"), teamPoints[ETeamIdentifier::Human], teamPoints[ETeamIdentifier::Alien]);
				return true;
			}
			else if (teamPoints[ETeamIdentifier::Alien] - teamPoints[ETeamIdentifier::Human] > 9)
			{
				UE_LOG(LogTemp, Warning, TEXT("Mercy Rule, Humans %d, Aliens %d"), teamPoints[ETeamIdentifier::Human], teamPoints[ETeamIdentifier::Alien]);
				return true;
			}
			break;
			default: 
			break;
		}
	}
	return false;
}