// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. Not needed rn
	//PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Root);
	
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFlag::PickUp_Implementation);
	InitLocation = GetActorLocation();
}

void AFlag::PickUp_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//checks if the OtherActor is a pawn
	APawn* isPawn = Cast<APawn>(OtherActor);

	if (isPawn) {
		//if it is, it checks if it has a playerState
		ACTFPlayerState* hasPlayerState = isPawn->GetPlayerState<ACTFPlayerState>();

		//if it does, attach flag
		if (hasPlayerState)	{
			USceneComponent* PlayerMesh = OtherActor->FindComponentByClass<USkeletalMeshComponent>();
			
			
			//attach the flag to the socket "FlagHolder" on the character mesh
			this->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, (TEXT("FlagHolder")));

			//Enables input so we can test the drop (drop is in blueprints for now)
			this->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
		}
		
	}
}




