// Fill out your copyright notice in the Description page of Project Settings.


#include "testComponent.h"

// Sets default values for this component's properties
UtestComponent::UtestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UtestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UtestComponent::testFunction_Implementation(FVector location, float number)
{
}


// Called every frame
void UtestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

