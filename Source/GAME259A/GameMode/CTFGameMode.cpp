// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "MiniFlag.h"
#include "Team.h"
#include "CapturePoint.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "GameFramework/HUD.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "Kismet/GameplayStatics.h"

ACTFGameMode::ACTFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Game_BP/BaseCharacter_BP"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> gameHudClass(TEXT("/Game/Game_BP/GameMode/BP_CTFHUD"));
	if(gameHudClass.Class != nullptr)
	{
		HUDClass =  gameHudClass.Class; //gameHudClass.Class;
	}

	GameStateClass = ACTFGameState::StaticClass();
	PlayerStateClass = ACTFPlayerState::StaticClass();
	
	//make sure GM can tick
	PrimaryActorTick.bCanEverTick = false;
	roundTimerTime = 20.0f;
	maxRounds = 5;
	currentRound = 1;
	timeBetweenFlagSpawns = 10.0f;
	requiredMiniFlags = 6;
	miniFlag = AMiniFlag::StaticClass();
	maxPoints = 0;
	isGameStarted = false;
}

void ACTFGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (miniFlag && mainFlag){
		maxPoints = (miniFlag.GetDefaultObject()->pointValue * requiredMiniFlags) + mainFlag.GetDefaultObject()->pointValue;
	}
	ctfGameState = Cast<ACTFGameState>(GameState);
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapturePoint::StaticClass(), foundActors);

	for(int i = 0; i < foundActors.Num(); i++)
	{
		ACapturePoint* cp = Cast<ACapturePoint>(foundActors[i]);
		ctfGameState->capturePoints.Add(cp);
	}
	
	GetWorldTimerManager().SetTimer(startGameTimer, this, &ACTFGameMode::BeginFirstRound, 10.0f);
}

void ACTFGameMode::BeginFirstRound()
{
	InitTeams();

}

FTimerHandle updateTimerHandle;

void ACTFGameMode::InitTeams()
{
	
	isGameStarted = true;
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
		ctfGameState->listOfTeams.Add(currentTeam);
	}

	ctfGameState->listOfTeams.Sort(
		[&](const ATeam& team1, const ATeam& team2)
			{
				return team1.teamID < team2.teamID;
			});

	for(int i = 0; i < ctfGameState->listOfTeams.Num(); i++)
	{
		teamPoints.Add(&ctfGameState->listOfTeams[i]->points);
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
		SpawnAllPlayersOnTeam(team->teamID);
	}

	ctfGameState->currentRound = currentRound;
	ctfGameState->maxRounds = maxRounds;

	GetWorldTimerManager().SetTimer(flagSpawnTimer, this, &ACTFGameMode::SpawnMiniFlag, timeBetweenFlagSpawns, true);
	GetWorldTimerManager().SetTimer(roundTimerHandle, this, &ACTFGameMode::EndRound, roundTimerTime);
	GetWorldTimerManager().SetTimer(updateTimerHandle, this, &ACTFGameMode::UpdateGameStateTime, 1.0f, true);
}

void ACTFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ACTFPlayerState* playerState = NewPlayer->GetPlayerState<ACTFPlayerState>();
	if(playerState)
	{
		playerState->teamScoreDelegate.AddDynamic(this, &ACTFGameMode::AddPoints);

		if(isGameStarted)
		{
			if(ctfGameState->GetTeam(ETeamIdentifier::Human)->players.Num() <= ctfGameState->GetTeam(ETeamIdentifier::Alien)->players.Num())
			{
				ctfGameState->ChooseTeam(ETeamIdentifier::Human, playerState);
            }
			else
            {
				ctfGameState->ChooseTeam(ETeamIdentifier::Alien, playerState);
            }

			playerState->OnRespawn();
		}
	}
}

void ACTFGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ACTFGameMode::UpdateGameStateTime()
{
	ctfGameState->timeLeft = FMath::RoundToInt(GetWorldTimerManager().GetTimerRemaining(roundTimerHandle));
}

void ACTFGameMode::SpawnMiniFlag()
{
	ATeam* humanTeam = ctfGameState->GetTeam(ETeamIdentifier::Human);
	ATeam* alienTeam = ctfGameState->GetTeam(ETeamIdentifier::Alien);
	
	if((humanTeam->miniFlagSpawnPoints.Num() != 0) && (alienTeam->miniFlagSpawnPoints.Num() != 0))
	{
		if(spawnedMiniFlags < requiredMiniFlags)
		{
			spawnedMiniFlags++;
			FVector spawnPoint;
			UE_LOG(LogTemp, Warning, TEXT("Mini flag number %d was spawned"), spawnedMiniFlags);
			if(spawnedMiniFlags % 2 == 0)
			{
				const int randomSpawn = FMath::RandRange(0, humanTeam->miniFlagSpawnPoints.Num() - 1);
				spawnPoint = humanTeam->miniFlagSpawnPoints[randomSpawn]->GetActorLocation();

			} else
			{
				const int randomSpawn = FMath::RandRange(0, alienTeam->miniFlagSpawnPoints.Num() - 1);
				spawnPoint = alienTeam->miniFlagSpawnPoints[randomSpawn]->GetActorLocation();
			}
			AFlag* flag = Cast<AFlag>(GetWorld()->SpawnActor(miniFlag, &spawnPoint));
			ctfGameState->activeFlags.Add(flag);
			ctfGameState->activeFlagsChangedDelegate.Broadcast(ctfGameState->activeFlags);
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
	GetWorldTimerManager().ClearTimer(flagSpawnTimer);
	GetWorldTimerManager().ClearTimer(roundTimerHandle);
	
	//Reduces the amount of rounds left
	currentRound++;
	ctfGameState->currentRound = currentRound;
	

	//Prints out how many rounds are left
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(currentRound) + " rounds remaining");

	ETeamIdentifier winner = WinCheck();
	
	//Checks to see if there is a winner. Prints out winner if it exists.
	if (winner == ETeamIdentifier::None)
	{
		RoundReset();
	}
	else if (winner == ETeamIdentifier::Human)
	{
		UE_LOG(LogTemp, Display, TEXT("Humans win"));
	}
	else 
		UE_LOG(LogTemp, Display, TEXT("Aliens win"));
}

//Resets all the actors in the rounds
//Current only debug messages and a timer reset
void ACTFGameMode::RoundReset() {

	spawnedMiniFlags = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Flags respawn."));

	for (int i = 0; i < ctfGameState->activeFlags.Num(); i++)
	{
		if(IsValid(ctfGameState->activeFlags[i]))
		{
			GetWorld()->DestroyActor(ctfGameState->activeFlags[i]);
		}
	}
	ctfGameState->activeFlags.Empty();
	
	for (auto team : ctfGameState->listOfTeams)
	{
		SpawnAllPlayersOnTeam(team->teamID);
	}

	for(auto capPoint : ctfGameState->capturePoints)
	{
		capPoint->RoundReset();
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Players respawn."));
	GetWorldTimerManager().SetTimer(flagSpawnTimer, this, &ACTFGameMode::SpawnMiniFlag, timeBetweenFlagSpawns, true);
	GetWorldTimerManager().SetTimer(roundTimerHandle, this, &ACTFGameMode::EndRound, roundTimerTime);
}


//Returns winning team for end of the game or mercy rule if applicable (returns none if mercy rule is not met)
//Does not calculate if their is a tie at the end. The winner will just be aliens.
ETeamIdentifier ACTFGameMode::WinCheck()
{
	if(teamPoints.Num() != 0)
	{
		if (abs(*teamPoints[static_cast<int32>(ETeamIdentifier::Human)] - *teamPoints[static_cast<int32>(ETeamIdentifier::Alien)]) > ((maxRounds - currentRound) * maxPoints) || maxRounds == currentRound)
		{
			return *teamPoints[static_cast<int32>(ETeamIdentifier::Human)] > *teamPoints[static_cast<int32>(ETeamIdentifier::Alien)] ? ETeamIdentifier::Human : ETeamIdentifier::Alien;
		}
		else
			return ETeamIdentifier::None;
	}
	else
		return ETeamIdentifier::None;
}

void ACTFGameMode::EndGame() {
	/*on screen debug message*/
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("GAME ENDED!"));

	/*clear teams
	reset teams list back to empty*/
	ctfGameState->GetTeam(ETeamIdentifier::Alien)->players.Empty();
	ctfGameState->GetTeam(ETeamIdentifier::Human)->players.Empty();

	/*open lobby level use base map as placeholder for now*/
	UGameplayStatics::OpenLevel(GetWorld(), "Maps/FinalMaps/L_Adventure_DAOC");
}


void ACTFGameMode::AddPoints_Implementation(ETeamIdentifier team, int32 value)
{
	ctfGameState->GetTeam(team)->AddPoints(value);
}

void ACTFGameMode::SpawnAllPlayersOnTeam(ETeamIdentifier team)
{
	ATeam* teamToSpawn = ctfGameState->GetTeam(team);
	if(teamToSpawn)
	{
		for(int i = 0; i < teamToSpawn->players.Num(); i++)
		{
			teamToSpawn->players[i]->OnRespawn();
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAllPlayersOnTeam function in the gamemode could not find team %d"), static_cast<int32>(team));
	}
}