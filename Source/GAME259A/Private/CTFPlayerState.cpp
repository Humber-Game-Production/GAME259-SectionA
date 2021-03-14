// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerState.h"
//#include "GAME259A/GameMode/PickUpAndDrop.h"

ACTFPlayerState::ACTFPlayerState(): teamID(ETeamIdentifier::Human), pointsEarned(0), kills(0), deaths(0),
flagsCaptured(0), FlagHeld(nullptr), PlayerCanPickupFlag(true)
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

void ACTFPlayerState::SetFlagHeld(AFlag* FlagHeld_)	{
	FlagHeld = FlagHeld_;
}

void ACTFPlayerState::SetCanPickupFlag(bool PlayerCanPickupFlag_)	{
	PlayerCanPickupFlag = PlayerCanPickupFlag_;
}

bool ACTFPlayerState::GetCanPickupFlag() const	{
	return PlayerCanPickupFlag;
}

void ACTFPlayerState::Death()	{

	IPickUpAndDrop* isFlag = Cast<IPickUpAndDrop>(FlagHeld);
	if(isFlag)	{
		isFlag->Execute_Drop(FlagHeld);
		PlayerCanPickupFlag = false;
		FlagHeld = nullptr;
	}
}