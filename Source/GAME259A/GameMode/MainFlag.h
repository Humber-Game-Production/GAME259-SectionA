// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Flag.h"
#include "MainFlag.generated.h"

/**
 * 
 */
UCLASS()
class GAME259A_API AMainFlag : public AFlag
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void BuildingMainFlag(int32 miniProgress_);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CompleteMainFlag();

	UFUNCTION(BlueprintImplementableEvent)
	void ResetMainFlag();

protected:
	//Flag pickup when the capsule overlaps with the player(OtherActor)
	AMainFlag();
	
	virtual void PickUp_Implementation(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void Drop_Implementation() override;


	virtual void Capture_Implementation() override;

	UFUNCTION(NetMulticast, Reliable)
	void ReserveFlag();
	
};


