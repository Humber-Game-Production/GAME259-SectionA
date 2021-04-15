// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
//#include "Components/CableComponent.h"
#include "GrapplingHook.generated.h"

UCLASS()
class GAME259A_API AGrapplingHook : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrapplingHook();

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

	UPROPERTY(BluePrintReadWrite, EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* SpawnEffect;

	UPROPERTY(Replicated)
		AActor* BaseCharacter;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere, Category = "Speeds")
		float Speed;
	UPROPERTY(EditAnywhere, Category = "Speeds")
		float SpeedVertical;
	UPROPERTY(EditAnywhere, Category = "Speeds")
		float LaunchSpeed;
	UPROPERTY(EditAnywhere, Category = "Speeds")
		float Gravity;

	UPROPERTY(EditAnywhere, Category = "Beam")
		FVector BeamLocation;

	//UPROPERTY(EditAnywhere, Category = "Rope")
		//UCableComponent* cable;

	UPROPERTY(EditAnywhere)
		FTimerHandle GrappleTimer;
private:

	UFUNCTION()
	void OnDestroy();
};
