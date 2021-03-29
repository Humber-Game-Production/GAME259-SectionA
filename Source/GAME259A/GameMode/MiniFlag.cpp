// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniFlag.h"


void AMiniFlag::Drop_Implementation()
{
	AFlag::Drop_Implementation();
	owningTeam = ETeamIdentifier::None;
	ChangeColour();
}

void AMiniFlag::Capture()	{
	AFlag::Capture();
	this->Destroy();
}
