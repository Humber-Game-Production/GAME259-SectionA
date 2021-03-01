// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include "GAME259A/BaseCharacter.h"
#include "GAME259A/Public/CTFPlayerState.h"

ATeam::ATeam()
{
	teamID = ETeamIdentifier::Human;
	capturePoint = nullptr;
	points = 0;
	miniFlagsColllected = 0;
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
}

void ATeam::SpawnPlayers()
{
	
}

void ATeam::SpawnPlayer(AActor* player)
{
	
}
