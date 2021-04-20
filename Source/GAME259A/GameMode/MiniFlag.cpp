// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniFlag.h"

#include "CTFGameMode.h"
#include "CTFGameState.h"


void AMiniFlag::Drop_Implementation()
{
	AFlag::Drop_Implementation();
	owningTeam = ETeamIdentifier::None;
	if(HasAuthority())
	{
		ChangeColour();
	}
}

void AMiniFlag::Capture()	{
	AFlag::Capture();
	if(HasAuthority())
	{
		if(GetWorld())
		{
			if(ACTFGameState* gameState = GetWorld()->GetGameState<ACTFGameState>())
			{
				if(gameState->activeFlags.Contains(this))
				{
					gameState->activeFlags.Remove(this);
				}
			}
		}
	}
	this->Destroy();
}
