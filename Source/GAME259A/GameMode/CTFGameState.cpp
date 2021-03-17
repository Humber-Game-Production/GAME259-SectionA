// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameState.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "CTFGameMode.h"

ACTFGameState::ACTFGameState()
{
	
}

void ACTFGameState::BeginPlay()
{
	Super::BeginPlay();

	//This needs to be changed later, delays Team initialization so that network and actors can be synced
	//Needs to be deterministic instead of after 3 seconds.
	if(HasAuthority())
	{
		GetWorldTimerManager().SetTimer(timer, this, &ACTFGameState::InitTeams, 1.0f);
	}
}



void ACTFGameState::ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player)
{
	if(listOfTeams.Contains(team))
	{
		listOfTeams[team]->AddPlayer(player);
		UE_LOG(LogTemp, Warning, TEXT("PlayerController %d Added to team %d"), player->GetUniqueID(), team);
	}
	
	Cast<ACTFPlayerState>(player)->teamScoreDelegate.AddDynamic(this, &ACTFGameState::AddPoints);
}

void ACTFGameState::PlayerLeft(AController* player)
{
	
}

void ACTFGameState::AddPoints(ETeamIdentifier team, int32 points)
{
	if(listOfTeams.Contains(team))
	{
		listOfTeams[team]->AddPoints(points);
	}
}

void ACTFGameState::InitTeams()
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing teams"));
	TArray<AActor*> teamsInLevel;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam::StaticClass(), teamsInLevel);

	for(int i = 0; i < teamsInLevel.Num(); i++)
	{
		ATeam* currentTeam = Cast<ATeam>(teamsInLevel[i]);
		listOfTeams.Add(currentTeam->teamID, currentTeam);
		Cast<ACTFGameMode>(AuthorityGameMode)->teamPoints.Add(currentTeam->teamID, &listOfTeams[currentTeam->teamID]->points);
	}


	Cast<ACTFGameMode>(AuthorityGameMode)->ctfGameState = this;
	
	for(int i = 0; i < PlayerArray.Num(); i++)
	{
		if(i % 2 == 0)
		{
			ChooseTeam(ETeamIdentifier::Human, Cast<ACTFPlayerState>(PlayerArray[i]));
		} else
		{
			ChooseTeam(ETeamIdentifier::Alien, Cast<ACTFPlayerState>(PlayerArray[i]));
		}
	}

	for (auto team : listOfTeams)
	{
			team.Value->SpawnPlayers();
	}
}