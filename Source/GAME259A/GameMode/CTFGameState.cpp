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
	DOREPLIFETIME( ACTFGameState, capturedFlags);
	DOREPLIFETIME( ACTFGameState, requiredFlags);
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

void ACTFGameState::OnPlayerLeft_Implementation(APlayerState* player)
{
	playerLeftDelegate.Broadcast(player);
}

void ACTFGameState::OnPlayerJoined_Implementation(APlayerState* player)
{
	playerJoinedDelegate.Broadcast(player);
}

void ACTFGameState::FlagDropped_Implementation(AFlag* flag)
{
	flagDropDelegate.Broadcast(flag);
}

void ACTFGameState::FlagPickedUp_Implementation(AFlag* flag)
{
	flagPickupDelegate.Broadcast(flag);
}

void ACTFGameState::OnRep_capturedFlags_Implementation()
{
	CapturedFlagDelegate.Broadcast(capturedFlags);
}

void ACTFGameState::OnGameEnd_Implementation(ETeamIdentifier winningTeam)
{
	gameEndDelegate.Broadcast(winningTeam);
}

void ACTFGameState::OnRoundEnd_Implementation()
{
	roundEndDelegate.Broadcast();
}

void ACTFGameState::OnRoundStart_Implementation()
{
	roundStartDelegate.Broadcast();
}

void ACTFGameState::OnRep_activeFlags_Implementation()
{
	activeFlagsChangedDelegate.Broadcast(activeFlags);
}
