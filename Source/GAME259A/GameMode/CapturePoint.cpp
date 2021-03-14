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
	teamID = ETeamIdentifier::None;
}

// Called when the game starts or when spawned
void ACapturePoint::BeginPlay()
{
	Super::BeginPlay();
	
	captureCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACapturePoint::OnHit);
	if (mainFlag != NULL)
	{
		//sets flag to be off the game field until it is generated
		mainFlag->SetActorRelativeLocation(FVector(0, -1000, 0));
		AFlag* flagMain = Cast<AFlag>(mainFlag);
		flagMain->InitLocation = flagMain->GetActorLocation();
	}
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
	

	if(playerC)
	{
		ACTFPlayerState* player = playerC->GetPlayerState<ACTFPlayerState>();

		//If the other actor has a ACTFPlayerState then we're able to do a check for their team
		if(player)
		{
			const ETeamIdentifier playersTeam = player->teamID;	

			//If the cap point's team matches the player's team then we do the point logic
			if ((playersTeam == teamID) || (teamID == ETeamIdentifier::None) && (OtherActor != this) && (OtherComp != NULL))
			{
				if (player->FlagHeld != nullptr) {
					if (player->FlagHeld->pointValue < 50) {
						AddPoints(player->FlagHeld->pointValue, player);
						player->FlagHeld->InitLocation = FVector(0, -1000, 0);
						player->PlayerDropFlag();
					}
					else if ((player->FlagHeld->pointValue >= 50) && (MainFlagCreator == true)) {
						flagsCaptured++;
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Flags captured: " + FString::FromInt(flagsCaptured));
						CheckForFlagConstruction();
						AddPoints(player->FlagHeld->pointValue, player);
						player->FlagHeld->InitLocation = FVector(0, -1000, 0);
						player->PlayerDropFlag();
					}
				}
				UE_LOG(LogTemp, Warning, TEXT("CapPoint Entered"));
			}
		}
	}
}

void ACapturePoint::CheckForFlagConstruction()
{
	if (MainFlagCreator == true) {
		UE_LOG(LogTemp, Warning, TEXT("This is the main flag spot"));
		if (flagsCaptured == 6) {

			mainFlag->SetActorRelativeLocation(FVector(this->GetActorLocation()));
			//main flag spawns on top of capture point
			//mainFlag->SetActorEnableCollision(false);
		}
	}
}

void ACapturePoint::AddPoints(int32 points, ACTFPlayerState* player)
{
	//Add points from team
	player->AddScore(points);
}

void ACapturePoint::RoundReset()
{
	flagsCaptured = 0;
	if ((MainFlagCreator == true) && (mainFlag != NULL)) {
		mainFlag->SetActorRelativeLocation(FVector(0, -1000, 0));
	}
}

