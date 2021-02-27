// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Root);

	//Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnCapsuleOBeginOverlap_Implementation);
	
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnCapsuleOBeginOverlap_Implementation);
}

void AFlag::OnCapsuleOBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//checks if the OtherActor also has the Interaface, 
	IPickUpAndDrop* OverlapOccurred = Cast<IPickUpAndDrop>(OtherActor);

	//if it does not, wont do this
	if (OverlapOccurred)
	{
		USceneComponent* PlayerMesh = OtherActor->FindComponentByClass<USkeletalMeshComponent>();

		//attach the flag to the socket "FlagHolder" on the character mesh
		this->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, (TEXT("FlagHolder")));
	}
	
}

