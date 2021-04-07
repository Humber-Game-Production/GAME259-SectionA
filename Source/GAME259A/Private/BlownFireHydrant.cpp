// Fill out your copyright notice in the Description page of Project Settings.


#include "BlownFireHydrant.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABlownFireHydrant::ABlownFireHydrant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HydrantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HydrantMesh"));
	HydrantMesh->SetSimulatePhysics(true);
	HydrantMesh->SetNotifyRigidBodyCollision(true);
	SetRootComponent(HydrantMesh);
	collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	collider->SetCapsuleSize(50.0f, 100.0f);
	collider->SetSimulatePhysics(false);
	collider->SetupAttachment(RootComponent);
	collider->SetNotifyRigidBodyCollision(false);
	
	HydrantMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABlownFireHydrant::BeginPlay()
{
	Super::BeginPlay();
	collider->OnComponentBeginOverlap.AddDynamic(this, &ABlownFireHydrant::BeginOverlap);

	//Uncomment this code to enable sound to play on the hydrants.
	//UGameplayStatics::PlaySoundAtLocation(this, soundFX, this->GetActorLocation(),this->GetActorRotation(), 0.5f, 1.0, 0.0f);
}

// Called every frame
void ABlownFireHydrant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlownFireHydrant::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag(FName("Player")))
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	
		if(!character->bRecentlyLaunched)
		{
			character->LaunchCharacter(character->GetActorUpVector() * 1500.0f, false, true);
			character->bRecentlyLaunched = true;
		}
		
		GetWorld()->GetTimerManager().SetTimer(resetLaunchTimer, [this, character]()
		{
			character->bRecentlyLaunched = false;
		},
		 0.5f, false);
	}
}

