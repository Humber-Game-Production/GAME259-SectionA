// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAME259A/BaseCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "BlownFireHydrant.generated.h"

UCLASS()
class GAME259A_API ABlownFireHydrant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlownFireHydrant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle resetLaunchTimer;

	UPROPERTY()
	UCapsuleComponent* collider;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* soundFX;
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* waterSpew;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HydrantMesh;
	
	UFUNCTION()
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
