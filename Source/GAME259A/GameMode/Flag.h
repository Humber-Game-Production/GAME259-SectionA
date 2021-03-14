// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PickUpAndDrop.h"
#include "GAME259A/Public/CTFPlayerState.h"

#include "Flag.generated.h"

UCLASS(BlueprintType)
class GAME259A_API AFlag : public AActor, public IPickUpAndDrop
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlag();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Flag pickup when the capsule overlaps with the player(OtherActor)
	UFUNCTION()
	void PickUp_Implementation(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	/*UFUNCTION()
	void Drop_Implementation() override{}*/
	
public:	
	// ( FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult)


	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 pointValue;

	UPROPERTY(BlueprintReadOnly)
	FVector InitLocation;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector DistanceFromGround;
};
