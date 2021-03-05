// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpAndDrop.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpAndDrop : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAME259A_API IPickUpAndDrop
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//call for collision
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PickUp(UPrimitiveComponent* OverlappedComponent, 
AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	//insert drop here
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Drop();
};
