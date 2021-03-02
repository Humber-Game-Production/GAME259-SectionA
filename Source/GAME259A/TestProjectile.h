// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TestProjectile.generated.h"


UCLASS()
class GAME259A_API ATestProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Sphere Collision Component **ASSUMING ALL PROJECTILES HAVE COLLISION**
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UCapsuleComponent* CollisionComponent;

	//Projectile Movement Component
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Root)
	USceneComponent* SceneRoot;

};
