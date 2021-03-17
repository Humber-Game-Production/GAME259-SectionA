// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportAbilityComponent.h"
#include "Engine/World.h"


void UTeleportAbilityComponent::SpawnActor(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_)
{
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, Transform_.GetLocation(), Transform_.Rotator());
	//GetOwner()->SetActorLocation()
}
