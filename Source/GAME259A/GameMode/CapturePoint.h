// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "CapturePoint.generated.h"

class ACTFPlayerState;

UCLASS(Blueprintable)
class GAME259A_API ACapturePoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* captureCollisionComp;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	UPROPERTY(Replicated, EditAnywhere)
	int32 flagsCaptured;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 requiredFlags;
	
	UPROPERTY(EditAnywhere)
	bool MainFlagCreator;

	UPROPERTY(EditAnywhere)
	class AActor* mainFlag;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	float flagInactivePeriod;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* flagSpawnEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* teamWinEffect;

	
	FTimerHandle mainFlagActiveTimer;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	ETeamIdentifier teamID;

	// Sets default values for this actor's properties
	ACapturePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void OnHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void CheckForFlagConstruction();

	UFUNCTION(NetMulticast, Reliable)
	void RoundReset();

	UFUNCTION()
	void SetMainFlagActive();


};
