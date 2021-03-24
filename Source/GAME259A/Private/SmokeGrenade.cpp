// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenade.h"

#include <concrt.h>


#include "Kismet/GameplayStatics.h"

//Set defaults
ASmokeGrenade::ASmokeGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SmokeGrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SmokeGrenadeMesh"));
	SmokeGrenadeMesh->SetSimulatePhysics(true);
	SmokeGrenadeMesh->SetNotifyRigidBodyCollision(true);
	SetRootComponent((SmokeGrenadeMesh));
	SmokeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticleSystem"));
	SmokeParticle->SetSimulatePhysics(false);
	SmokeParticle->SetupAttachment(RootComponent);
}

//Set smoke to not spawn and setup collision.
void ASmokeGrenade::BeginPlay()
{
	Super::BeginPlay();
	SmokeParticle->DeactivateSystem();
	SmokeGrenadeMesh->OnComponentHit.AddDynamic(this, &ASmokeGrenade::OnCompHit);
}

void ASmokeGrenade::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//When this grenade hits anything, make it start emitting smoke. Set timer for activation length.
void ASmokeGrenade::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(!bIsSpawned)
	{
		SmokeParticle->ActivateSystem();
		
		bIsSpawned = true;

		GetWorld()->GetTimerManager().SetTimer(EndSmokeTimer, this, &ASmokeGrenade::OnEndSmokeTimerFinish, 10.0f, 0.0f);
	}
}

//Deactivate smoke and set timer to delete grenade.
void ASmokeGrenade::OnEndSmokeTimerFinish()
{
	SmokeParticle->DeactivateSystem();
	GetWorld()->GetTimerManager().SetTimer(DeleteTimer, this, &ASmokeGrenade::OnDeleteTimerFinish, 2f, 0.0f);
}

//Delete grenade.
void ASmokeGrenade::OnDeleteTimerFinish()
{
	this->Destroy();
}
