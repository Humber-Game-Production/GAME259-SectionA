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

void ACTFGameState::AddPoints(ETeamIdentifier team, int32 points)
{
	if(listOfTeams.Contains(team))
	{
		listOfTeams[team]->AddPoints(points);
	}
}
