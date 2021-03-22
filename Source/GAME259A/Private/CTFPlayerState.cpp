// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerState.h"
#include "GAME259A/GameMode/Flag.h"
#include "Net/UnrealNetwork.h"

ACTFPlayerState::ACTFPlayerState(): teamID(ETeamIdentifier::Human), pointsEarned(0), kills(0), deaths(0),
                                    flagsCaptured(0), FlagHeld(nullptr), PlayerCanPickupFlag(true)
{

}

void ACTFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME( ACTFPlayerState, teamID);
	DOREPLIFETIME( ACTFPlayerState, pointsEarned);
	DOREPLIFETIME( ACTFPlayerState, kills);
	DOREPLIFETIME( ACTFPlayerState, deaths);
	DOREPLIFETIME( ACTFPlayerState, flagsCaptured);
	DOREPLIFETIME( ACTFPlayerState, FlagHeld);
	DOREPLIFETIME( ACTFPlayerState, PlayerCanPickupFlag);
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

void ACTFPlayerState::PlayerDropFlag()	{
	if(FlagHeld)
	{
		FlagHeld->Execute_Drop(FlagHeld);
		PlayerCanPickupFlag = true;
		FlagHeld = nullptr;
	}
}

void ACTFPlayerState::CaptureFlag()
{
	if(FlagHeld)
	{
		AddScore(FlagHeld->pointValue);
		FlagHeld->Capture();
		PlayerCanPickupFlag = true;
		FlagHeld = nullptr;
	}
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

void ACTFPlayerState::OnDeath()	{
	PlayerDropFlag();
	PlayerCanPickupFlag = false;
}
