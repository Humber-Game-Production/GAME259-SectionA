// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectile.h"

// Sets default values
ATestProjectile::ATestProjectile() : CollisionComponent(nullptr), ProjectileComp(nullptr), Mesh(nullptr), SceneRoot(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//We want actor to be root component...

	//Check 

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionComponent->InitCapsuleSize(0.f, 0.f);

	CollisionComponent->SetupAttachment(RootComponent);



	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComp->SetUpdatedComponent(RootComponent);

		//THIS IS STUFF THATS GONNA BE CHANGED BASED ON NEEDS JUST DEFAULT VALUES//
	ProjectileComp->InitialSpeed = 3000.0f;
	ProjectileComp->MaxSpeed = 3000.0f;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->bShouldBounce = true;
	ProjectileComp->Bounciness = 0.3f;
	ProjectileComp->ProjectileGravityScale = 1.0f;


}

// Called when the game starts or when spawned
void ATestProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	


}

