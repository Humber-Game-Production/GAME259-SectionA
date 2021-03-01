// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePoint.h"

#include "CTFPlayerState.h"
#include "GAME259A/GameMode/Team.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ACapturePoint::ACapturePoint()
{
	captureCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CaptureComp"));
	captureCollisionComp->InitSphereRadius(40.0f);
	captureCollisionComp->BodyInstance.SetCollisionProfileName("WorldDynamic");
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = captureCollisionComp;

}

// Called when the game starts or when spawned
void ACapturePoint::BeginPlay()
{
	Super::BeginPlay();
	
	captureCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACapturePoint::OnHit);
}

// Called every frame
void ACapturePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACapturePoint::OnHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APawn* playerC = Cast<APawn>(OtherActor);
	
	ACTFPlayerState* player = playerC->GetPlayerState<ACTFPlayerState>();
	
	if (player && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogTemp, Warning, TEXT("CapPoint Entered"));
		flagsCaptured++;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Flags captured: " + FString::FromInt(flagsCaptured));
		CheckForFlagConstruction();

		//PlaceHolder value until the Flags are connected to the players
		AddPoints(50, player);
	}
}

void ACapturePoint::CheckForFlagConstruction()
{
	if (MainFlagCreator == true) {
		UE_LOG(LogTemp, Warning, TEXT("This is the main flag spot"));
		if (flagsCaptured >= 6) {
			//generate main flag here
		}
	}
}

void ACapturePoint::AddPoints(int32 points, ACTFPlayerState* player)
{
	//Add points from team
	player->AddScore(points);
}

