// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameState.h"
#include "GAME259A/GameMode/Team.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "Net/UnrealNetwork.h"

ACTFGameState::ACTFGameState()
{
	bReplicates = true;

}

void ACTFGameState::BeginPlay()
{
	Super::BeginPlay();

}

void ACTFGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( ACTFGameState, listOfTeams);
	DOREPLIFETIME( ACTFGameState, flagHolders);
	DOREPLIFETIME( ACTFGameState, activeFlags);
	DOREPLIFETIME( ACTFGameState, capturePoints);
	DOREPLIFETIME( ACTFGameState, timeLeft);
	DOREPLIFETIME( ACTFGameState, currentRound);
	DOREPLIFETIME( ACTFGameState, maxRounds);
}


void ACTFGameState::ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player)
{
	listOfTeams[static_cast<int32>(team)]->AddPlayer(player);
	UE_LOG(LogTemp, Warning, TEXT("PlayerController %d Added to team %d"), player->GetUniqueID(), team);

}

void ACTFGameState::AddPoints(ETeamIdentifier team, int32 points)
{
	listOfTeams[static_cast<int32>(team)]->AddPoints(points);
}

ATeam* ACTFGameState::GetTeam(ETeamIdentifier team) const
{
	for(int i = 0; i < listOfTeams.Num(); i++)
	{
		if(listOfTeams[i]->teamID == team)
		{
			return listOfTeams[i];
		}
	}
	return nullptr;
}
