// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "MiniFlag.h"
#include "MainFlag.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "GameFramework/HUD.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "Kismet/GameplayStatics.h"

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
	
	roundTimerTime = 20.0f;
	maxRounds = 5;
	currentRound = 1;
	timeBetweenFlagSpawns = 10.0f;
	requiredMiniFlags = 6;
	miniFlag = AMiniFlag::StaticClass();
}

void ACTFGameMode::BeginPlay()
{
	Super::BeginPlay();



	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapturePoint::StaticClass(), foundActors);

	for(int i = 0; i < foundActors.Num(); i++)
	{
		capturePoints.Add(Cast<ACapturePoint>(foundActors[i]));
	}

	GetWorldTimerManager().SetTimer(startGameTimer, this, &ACTFGameMode::BeginFirstRound, 4.0f);
}

void ACTFGameMode::BeginFirstRound()
{
	InitTeams();

}

void ACTFGameMode::InitTeams()
{
	ctfGameState = Cast<ACTFGameState>(GameState);
	
	UE_LOG(LogTemp, Warning, TEXT("Initializing teams"));
	TArray<AActor*> teamsInLevel;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam::StaticClass(), teamsInLevel);

	if(teamsInLevel.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "There are no teams in the current level, GameMode will not run");
		UE_LOG(LogTemp, Error, TEXT("There are no teams in the current level, GameMode will not run"));
		return;
	}
	
	for(int i = 0; i < teamsInLevel.Num(); i++)
	{
		ATeam* currentTeam = Cast<ATeam>(teamsInLevel[i]);
		ctfGameState->listOfTeams.Add(currentTeam->teamID, currentTeam);
		teamPoints.Add(currentTeam->teamID, &ctfGameState->listOfTeams[currentTeam->teamID]->points);
	}
	
	for(int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		if(i % 2 == 0)
		{
			ctfGameState->ChooseTeam(ETeamIdentifier::Human, Cast<ACTFPlayerState>(GameState->PlayerArray[i]));
		} else
		{
			ctfGameState->ChooseTeam(ETeamIdentifier::Alien, Cast<ACTFPlayerState>(GameState->PlayerArray[i]));
		}
	}

	for (auto team : ctfGameState->listOfTeams)
	{
		team.Value->SpawnPlayers();
	}

	GetWorldTimerManager().SetTimer(flagSpawnTimer, this, &ACTFGameMode::SpawnMiniFlag, timeBetweenFlagSpawns, true);
	GetWorldTimerManager().SetTimer(roundTimerHandle, this, &ACTFGameMode::EndRound, roundTimerTime);
}

void ACTFGameMode::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);

	//Add timer to screen
	if (GetWorldTimerManager().TimerExists(roundTimerHandle) && GetWorldTimerManager().IsTimerPaused(roundTimerHandle) == false) {
		timeLeft = FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(roundTimerHandle));
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
}

void ACTFGameMode::SpawnMiniFlag()
{
	if((ctfGameState->listOfTeams[ETeamIdentifier::Human]->miniFlagSpawnPoints.Num() != 0) && (ctfGameState->listOfTeams[ETeamIdentifier::Alien]->miniFlagSpawnPoints.Num() != 0))
	{
		if(spawnedMiniFlags < requiredMiniFlags)
		{
			spawnedMiniFlags++;
			UE_LOG(LogTemp, Warning, TEXT("Mini flag number %d was spawned"), spawnedMiniFlags);
			if(spawnedMiniFlags % 2 == 0)
			{
				const int randomSpawn = FMath::RandRange(0, ctfGameState->listOfTeams[ETeamIdentifier::Human]->miniFlagSpawnPoints.Num() - 1);
				FVector spawnPoint = ctfGameState->listOfTeams[ETeamIdentifier::Human]->miniFlagSpawnPoints[randomSpawn]->GetActorLocation();
				GetWorld()->SpawnActor(miniFlag, &spawnPoint);
			} else
			{
				const int randomSpawn = FMath::RandRange(0, ctfGameState->listOfTeams[ETeamIdentifier::Alien]->miniFlagSpawnPoints.Num() - 1);
				FVector spawnPoint = ctfGameState->listOfTeams[ETeamIdentifier::Alien]->miniFlagSpawnPoints[randomSpawn]->GetActorLocation();
				GetWorld()->SpawnActor(miniFlag, &spawnPoint);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Last miniflag was already spawned"));
			GetWorldTimerManager().ClearTimer(flagSpawnTimer);
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Teams don't have specified flag spawn points"));
	}
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
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Team2 wins"));
			}
		}
		else
		{
			RoundReset();
		}
	}
}

//Resets all the actors in the rounds
//Current only debug messages and a timer reset
void ACTFGameMode::RoundReset() {

	spawnedMiniFlags = 0;
	GetWorldTimerManager().ClearTimer(flagSpawnTimer);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Flags respawn."));


	
	for (auto team : ctfGameState->listOfTeams)
	{
		team.Value->SpawnPlayers();
	}

	for(auto capPoint: capturePoints)
	{
		capPoint->RoundReset();
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Players respawn."));
	GetWorldTimerManager().SetTimer(roundTimerHandle, this, &ACTFGameMode::EndRound, roundTimerTime);
	GetWorldTimerManager().SetTimer(roundTimerHandle, this, &ACTFGameMode::EndRound, roundTimerTime);
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