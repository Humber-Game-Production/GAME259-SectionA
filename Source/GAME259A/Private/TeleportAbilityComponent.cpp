// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportAbilityComponent.h"
#include "Engine/World.h"

void UTeleportAbilityComponent::ActivateAbility_Implementation(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_)
{
	Super::ActivateAbility_Implementation(CoolDown_, Transform_, Duration_, Team_);

	GetWorld()->SpawnActor<AActor>(ActorToSpawn , Transform_.GetLocation(), Transform_.Rotator());
	//GetOwner()->SetActorLocation()
}

