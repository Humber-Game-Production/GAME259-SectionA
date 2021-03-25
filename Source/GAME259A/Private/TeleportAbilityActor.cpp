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

	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);
}
// Called when the game starts or when spawned
void ATeleportAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ATeleportAbilityActor::OnCompHit);
	//BaseCharacter = GetOwner();
	//ThrowInDirection();
	
}

// Called every frame
void ATeleportAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportAbilityActor::SetSpawner(AActor* BaseCharacter_)
{
	BaseCharacter = BaseCharacter_;
}

//sets objects velocity

void ATeleportAbilityActor::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));

	BaseCharacter->SetActorLocation(GetActorLocation() + (Hit.Normal * 120));
	this->Destroy();

}


