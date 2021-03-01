// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapturePoint.generated.h"

class ACTFPlayerState;

UCLASS()
class GAME259A_API ACapturePoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* captureCollisionComp;

	UPROPERTY(EditAnywhere)
	int flagsCaptured;
	UPROPERTY(EditAnywhere)
	bool MainFlagCreator;
	
public:	
	// Sets default values for this actor's properties
	ACapturePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void CheckForFlagConstruction();
	UFUNCTION()
	void AddPoints(int32 points, ACTFPlayerState* player);


};
