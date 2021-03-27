// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportAbilityActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATeleportAbilityActor::ATeleportAbilityActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(false);
	SetRootComponent(Mesh);
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetSphereRadius(7.0f);
	SphereCollider->SetSimulatePhysics(true);
	SphereCollider->SetupAttachment(RootComponent);
	SphereCollider->SetNotifyRigidBodyCollision(true);
}
// Called when the game starts or when spawned
void ATeleportAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentHit.AddDynamic(this, &ATeleportAbilityActor::OnCompHit);
	//BaseCharacter = GetOwner();
	//ThrowInDirection();
	
}

void ATeleportAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATeleportAbilityActor, BaseCharacter);
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
	FRotator CharacterRotation = FRotator(BaseCharacter->GetActorRotation().Pitch, BaseCharacter->GetActorRotation().Yaw, BaseCharacter->GetActorRotation().Roll - 90);
	FVector CharacterLocation = FVector(BaseCharacter->GetActorLocation().X, BaseCharacter->GetActorLocation().Y, BaseCharacter->GetActorLocation().Z-80);
	FVector TpLocation = FVector(Mesh->GetComponentLocation().X + (Hit.ImpactNormal.X * 120), Mesh->GetComponentLocation().Y + (Hit.ImpactNormal.Y * 120), Mesh->GetComponentLocation().Z);
	if (this) {
		if (BaseCharacter) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, CharacterLocation, CharacterRotation);
			BaseCharacter->TeleportTo(GetActorLocation() + (Hit.ImpactNormal * 120), BaseCharacter->GetActorRotation());
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, TpLocation, CharacterRotation);
		}
		if (Mesh != NULL)
		{

			Mesh->UnregisterComponent();
			Mesh->DestroyComponent(true);
		}
		if (SphereCollider) {
			SphereCollider->UnregisterComponent();
			SphereCollider->DestroyComponent(true);
		}
		MarkPendingKill();
	}
}