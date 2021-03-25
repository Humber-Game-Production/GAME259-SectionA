// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SmokeGrenade.generated.h"

UCLASS()
class GAME259A_API ASmokeGrenade final : public AActor
{
	GENERATED_BODY()
	
public:	
	ASmokeGrenade();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//Collision detection
	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smoke Visuals")
	UStaticMeshComponent* SmokeGrenadeMesh;		//Static mesh.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Smoke Visuals")
	UParticleSystemComponent* SmokeParticle;	//Particle system to spawn.
	
private:
	UPROPERTY()
	FTimerHandle EndSmokeTimer;	//Length of smoke
	UPROPERTY()
	FTimerHandle DeleteTimer;	//Length of actor

	bool bIsSpawned = false;	//Toggle if the smoke has been emitted or not. Used so smoke system only starts once.
	
	UFUNCTION()
	void OnEndSmokeTimerFinish();	//What happens when EndSmokeTimer is finished.
	UFUNCTION()
	void OnDeleteTimerFinish();		//What happens when DeleteTimer is finished.
};
