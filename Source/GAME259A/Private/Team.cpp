// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"

ATeam::ATeam()
{
	playerType = PlayerType::Human;
	capturePoint = nullptr;
	points = 0;
	miniFlagsColllected = 0;
}

void ATeam::AddPlayer(AActor* player_)
{
	players.Add(player_);
}

void ATeam::RemovePlayer(AActor* player_)
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
