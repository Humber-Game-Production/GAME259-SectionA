// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportAbilityComponent.h"
#include "Engine/World.h"


void UTeleportAbilityComponent::SpawnActor(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, FVector Velocity_)
{
	Vel = Velocity_;
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, Transform_.GetLocation(), Transform_.Rotator());
	//GetOwner()->SetActorLocation()
}

FVector UTeleportAbilityComponent::GetVelocity(FVector Velocity_)
{
	Velocity_ = Vel;
	return Velocity_;
}

void UTeleportAbilityComponent::CreatePredictionSpline()
{
	//FVector loc = ;
	if (isAiming) {
		//GetWorld()->SpawnActor<AActor>(EndPoint, FVector(0,0,0));
	}
}

void UTeleportAbilityComponent::StartCoolDown(float CoolDown_)
{
	if (CanUseAbility) {
		
		CanUseAbility = false;

		GetWorld()->GetTimerManager().SetTimer(AbilityTimerHandle, this, &UTeleportAbilityComponent::ResetAbilityCoolDown, CoolDown_, false);
	}
}

UTeleportAbilityComponent::UTeleportAbilityComponent()
{
	CanUseAbility = true;
}

void UTeleportAbilityComponent::ResetAbilityCoolDown()
{
	CanUseAbility = true;
}
