// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TeleportAbilityActor.generated.h"

UCLASS()
class GAME259A_API ATeleportAbilityActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportAbilityActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void SetSpawner(AActor* BaseCharacter_);

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
		USoundBase* TeleportAbilitySound;

	UPROPERTY(BluePrintReadWrite, EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* SpawnEffect;

	UPROPERTY(Replicated)
		AActor* BaseCharacter;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollider;
};
