// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void UTeleportAbilityComponent::SpawnActor(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, FVector Velocity_)
{
	Vel = Velocity_;
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Owner->GetActorForwardVector();
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, Transform_.GetLocation(), Transform_.Rotator(), spawnParams);
}

void UTeleportAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
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


void UTeleportAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Can Use Ability In %f"), GetWorld()->GetTimerManager().GetTimerRemaining(AbilityTimerHandle)));
}

UTeleportAbilityComponent::UTeleportAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CanUseAbility = true;
}

void UTeleportAbilityComponent::ResetAbilityCoolDown()
{
	CanUseAbility = true;
}