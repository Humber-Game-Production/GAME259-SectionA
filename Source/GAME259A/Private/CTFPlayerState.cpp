// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerState.h"

ACTFPlayerState::ACTFPlayerState(): teamID(ETeamIdentifier::Human), pointsEarned(0), kills(0), deaths(0), flagsCaptured(0)
{

}

void ACTFPlayerState::AddScore(int32 amountOfPoints)
{
	pointsEarned += amountOfPoints;
	teamScoreDelegate.Broadcast(teamID, amountOfPoints);
	UE_LOG(LogTemp, Warning, TEXT("%d points added to team %d"), amountOfPoints, teamID);
}

void ACTFPlayerState::ResetStats()
{
	pointsEarned = 0;

	kills = 0;

	deaths = 0;

	flagsCaptured = 0;
}

void ACTFPlayerState::SetTeam(ETeamIdentifier team)
{
	teamID = team;
}

