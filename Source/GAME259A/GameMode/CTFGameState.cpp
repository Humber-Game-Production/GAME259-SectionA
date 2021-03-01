// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GAME259A/Public/CTFPlayerState.h"

ACTFGameState::ACTFGameState()
{
	
}

void ACTFGameState::BeginPlay()
{
	Super::BeginPlay();

	InitTeams();
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
	TArray<AActor*> teamsInLevel;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam::StaticClass(), teamsInLevel);

	for(int i = 0; i < teamsInLevel.Num(); i++)
	{
		ATeam* currentTeam = Cast<ATeam>(teamsInLevel[i]);
		listOfTeams.Add(currentTeam->teamID, currentTeam);
	}

	for(int i = 0; i < PlayerArray.Num(); i++)
	{
		ChooseTeam(ETeamIdentifier::Human, Cast<ACTFPlayerState>(PlayerArray[i]));
	}
}