// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportAbilityActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATeleportAbilityActor::ATeleportAbilityActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//setting up Projectile stuff
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Mesh);

	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bSimulationEnabled = true;
	ProjectileMovement->bEditableWhenInherited = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bAutoActivate = true;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(1500, 1500, 1500));
}
// Called when the game starts or when spawned
void ATeleportAbilityActor::BeginPlay()
{
	Super::BeginPlay();

	//BaseCharacter = GetOwner();
	//ThrowInDirection();
	
}

// Called every frame
void ATeleportAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportAbilityActor::SetLocation(FVector Location_, AActor* BaseCharacter_)
{
	//BaseCharacter_->SetActorLocation(Location_, false, nullptr, ETeleportType::TeleportPhysics);
	this->Destroy();
}
//sets objects velocity




