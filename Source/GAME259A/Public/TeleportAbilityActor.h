// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void SetLocation(FVector Location_, AActor* BaseCharacter_);

	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USceneComponent* Root;
	UPROPERTY(BluePrintReadWrite, EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY()
		AActor* BaseCharacter;

};
