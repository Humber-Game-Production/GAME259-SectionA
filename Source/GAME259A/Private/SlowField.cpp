// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowField.h"
#include "..//BaseCharacter.h"

// Sets default values
ASlowField::ASlowField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SlowFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlowFieldMesh"));
	SlowFieldMesh->SetSimulatePhysics(true);
	SlowFieldMesh->SetNotifyRigidBodyCollision(true);
	SetRootComponent(SlowFieldMesh);
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetSphereRadius(0.0f);
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetupAttachment(RootComponent);
	SphereCollider->SetNotifyRigidBodyCollision(false);
}

// Called when the game starts or when spawned
void ASlowField::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ASlowField::BeginOverlap);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &ASlowField::EndOverlap);
	SlowFieldMesh->OnComponentHit.AddDynamic(this, &ASlowField::OnCompHit);
	SlowFieldMesh->SetSimulatePhysics(true);

}

// Called every frame
void ASlowField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlowField::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr) {
		if (Cast<ABaseCharacter>(OtherActor))Cast<ABaseCharacter>(OtherActor)->Slow();
	}
}

void ASlowField::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr) {
		if(Cast<ABaseCharacter>(OtherActor))Cast<ABaseCharacter>(OtherActor)->UnSlow();
	}

}

void ASlowField::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	SphereCollider->SetSphereRadius(500.0f, true);
	SphereCollider->SetNotifyRigidBodyCollision(true);
	GetWorld()->GetTimerManager().SetTimer(SlowFieldTimer, this, &ASlowField::OnTimerFinish, 6.0f, false);
	SlowFieldMesh->OnComponentHit.RemoveDynamic(this, &ASlowField::OnCompHit);

}

void ASlowField::OnTimerFinish()
{
	GetWorld()->GetTimerManager().ClearTimer(SlowFieldTimer);
	SphereCollider->SetSphereRadius(0.0f, true);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SlowFieldMesh->SetSimulatePhysics(false);
	SlowFieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollider->OnComponentBeginOverlap.RemoveDynamic(this, &ASlowField::BeginOverlap);
	SphereCollider->OnComponentEndOverlap.RemoveDynamic(this, &ASlowField::EndOverlap);
	if (SlowFieldMesh != NULL)
	{
		SlowFieldMesh->UnregisterComponent();
		SlowFieldMesh->DestroyComponent(true);
	}
	if (SphereCollider != NULL)
	{
		SphereCollider->UnregisterComponent();
		SphereCollider->DestroyComponent(true);
	}
	if (this != NULL)
	{
		MarkPendingKill();
	}
}





