// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAbilityClass.h"
#include "Kismet/GameplayStatics.h"


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

}

void UBaseAbilityClass::UseAbility(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_, FVector Velocity_)
{
	//!= NULL && Transform_.GetLocation() != NULL && Transform_.Rotator
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Called Ability %f"), Transform_.GetLocation().ToString()));

	FVector location = Transform_.GetLocation();
	FRotator Rotation = Transform_.Rotator();
	if (GetWorld()) {
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, location, Rotation);
	}
}

void UBaseAbilityClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}


