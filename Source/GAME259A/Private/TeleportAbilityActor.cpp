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
	if (this) {
		if(BaseCharacter) BaseCharacter->TeleportTo(GetActorLocation() + (Hit.ImpactNormal * 500), BaseCharacter->GetActorRotation(), false, true);

		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, BaseCharacter->GetActorLocation(), BaseCharacter->GetActorRotation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, Mesh->GetComponentLocation(), BaseCharacter->GetActorRotation());
		if (Mesh != NULL)
		{
			Mesh->UnregisterComponent();
			Mesh->DestroyComponent(true);
		}

		MarkPendingKill();
	}
}


