// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "GAME259A/GameMode/CapturePoint.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATeam::ATeam()
{
	teamID = ETeamIdentifier::Human;
	capturePoint = nullptr;
	points = 0;
	miniFlagsCollected = 0;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATeam::BeginPlay()
{
	if(capturePoint)
	{
		capturePoint->teamID = teamID;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Team %d's capture point is nullptr"), teamID);
	}
}

void ATeam::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( ATeam, points );
	DOREPLIFETIME( ATeam, playerType );
	DOREPLIFETIME( ATeam, players );
	DOREPLIFETIME( ATeam, respawnPoints );
	DOREPLIFETIME( ATeam, capturePoint );
	DOREPLIFETIME( ATeam, miniFlagSpawnPoints );
	DOREPLIFETIME( ATeam, teamID );
}

void ATeam::AddPlayer(ACTFPlayerState* player_)
{
	player_->SetTeam(teamID);
	players.Add(player_);
}

void ATeam::RemovePlayer(ACTFPlayerState* player_)
{
	players.Remove(player_);
}

void ATeam::AddPoints(int32 value)
{
	points += value;
	UE_LOG(LogTemp, Warning, TEXT("Team %d has %d points"), teamID, points);
}