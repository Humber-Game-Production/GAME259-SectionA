// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityClass.h"

// Sets default values for this component's properties
UBaseAbilityClass::UBaseAbilityClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UBaseAbilityClass::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UBaseAbilityClass::ActivateAbility_Implementation(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_)
{
}

void UBaseAbilityClass::UseAbility(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_)
{
	ActivateAbility(CoolDown_, Transform_, Duration_, Team_,Damage_);
}

// Called every frame
void UBaseAbilityClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

