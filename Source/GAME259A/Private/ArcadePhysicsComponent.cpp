// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadePhysicsComponent.h"

// Sets default values for this component's properties
UArcadePhysicsComponent::UArcadePhysicsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UArcadePhysicsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UArcadePhysicsComponent::MoveByCurve_Implementation(UCurveFloat* Curve, FVector Direction)
{
}

// Called every frame
void UArcadePhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

