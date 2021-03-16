// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniFlag.h"

void AMiniFlag::PickUp_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlag::PickUp_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMiniFlag::Drop_Implementation()
{
	AFlag::Drop_Implementation();
}

void AMiniFlag::Capture()
{
	AFlag::Capture();
	this->Destroy();
}
