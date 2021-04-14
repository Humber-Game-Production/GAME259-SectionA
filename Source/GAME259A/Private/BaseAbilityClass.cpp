// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseAbilityClass.h"
#include "../Public/TeleportAbilityActor.h"
#include "Kismet/GameplayStatics.h"
#include <GrapplingHook.h>


// Sets default values for this component's properties
UBaseAbilityClass::UBaseAbilityClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBaseAbilityClass::BeginPlay()
{
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeleportAbilityActor::StaticClass(), FoundActors);
	Super::BeginPlay();
}

void UBaseAbilityClass::UseAbility(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_, FVector Velocity_, AActor* Spawner_)
{
	//!= NULL && Transform_.GetLocation() != NULL && Transform_.Rotator
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Called Ability %f"), Transform_.GetLocation().ToString()));

	FVector location = Transform_.GetLocation();
	FRotator Rotation = Transform_.Rotator();
	AActor* AbilityActor;
	if (GetWorld()) {
		AbilityActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, location, Rotation);
		//AbilityActor->FindComponentByClass<UStaticMeshComponent>()->AddImpulse(Velocity_);
		if(AbilityActor)
		{
			AbilityActor->FindComponentByClass<UStaticMeshComponent>()->SetPhysicsLinearVelocity(Velocity_);
			if(Cast<ATeleportAbilityActor>(AbilityActor))
				Cast<ATeleportAbilityActor>(AbilityActor)->SetSpawner(Spawner_);
			if (Cast<AGrapplingHook>(AbilityActor))
				Cast<AGrapplingHook>(AbilityActor)->SetSpawner(Spawner_);
		}
	}
}

void UBaseAbilityClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


