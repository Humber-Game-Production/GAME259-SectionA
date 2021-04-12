// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePoint.h"


#include "CTFGameMode.h"
#include "CTFPlayerState.h"
#include "GAME259A/GameMode/Team.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "MainFlag.h"
#include "MiniFlag.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACapturePoint::ACapturePoint() : flagsCaptured(0), requiredFlags(6)
{
	captureCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CaptureComp"));
	captureCollisionComp->InitSphereRadius(40.0f);
	captureCollisionComp->BodyInstance.SetCollisionProfileName("WorldDynamic");
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = captureCollisionComp;
	teamID = ETeamIdentifier::None;

	//SetReplicates(true);
	//SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void ACapturePoint::BeginPlay()
{
	Super::BeginPlay();
	
	captureCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACapturePoint::OnHit);

	if(mainFlag)
	{
		mainFlag->SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 60.0f));
	}
	
	if(HasAuthority())
	{
		if(ACTFGameMode* ctfGameMode = GetWorld()->GetAuthGameMode<ACTFGameMode>())
		{
			requiredFlags = ctfGameMode->requiredMiniFlags;
		}
	}
}

void ACapturePoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACapturePoint, flagsCaptured);
	DOREPLIFETIME(ACapturePoint, requiredFlags);
	DOREPLIFETIME(ACapturePoint, flagInactivePeriod);
	DOREPLIFETIME(ACapturePoint, teamID);
}

// Called every frame
void ACapturePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACapturePoint::OnRep_flagsCaptured()
{
	TestFunction();
}

void ACapturePoint::TestFunction_Implementation()	{
	if(mainFlag)	{
		mainFlag->BuildingMainFlag(flagsCaptured);
	}
}

void ACapturePoint::OnHit_Implementation(UPrimitiveComponent* OverlappedComponent,
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

			//If the player's holding a flag
			if(player->FlagHeld)
			{
				UE_LOG(LogTemp, Warning, TEXT("CapPoint Entered"));
				//if this is the midpoint and the player's holding a miniflag
				if(Cast<AMiniFlag>(player->FlagHeld) && (MainFlagCreator == true))
				{
					flagsCaptured++;
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Flags captured: " + FString::FromInt(flagsCaptured));
					CheckForFlagConstruction();
					player->FlagHeld->InitLocation = FVector(0, -1000, 0);
					player->CaptureFlag();
					UE_LOG(LogTemp, Warning, TEXT("MiniFlag number %d captured at Midpoint"), flagsCaptured);
					
					//Main Flag building in here
					if(HasAuthority())
					{
						TestFunction();
					}
					
				} //if the player's holding a main flag, this capture point is part of the same team as the player, and this is not the midPoint
				else if(Cast<AMainFlag>(player->FlagHeld) && (playersTeam == teamID) && (MainFlagCreator == false))
				{
					player->FlagHeld->InitLocation = FVector(0, -1000, 0);
					player->CaptureFlag();
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), teamWinEffect, this->GetActorLocation());
					UE_LOG(LogTemp, Warning, TEXT("MainFlag captured at team %d's capture point"), teamID);
					if (HasAuthority())
					{
						if (ACTFGameMode* ctfGameMode = GetWorld()->GetAuthGameMode<ACTFGameMode>())
						{
							ctfGameMode->RoundReset();
						}
					}
				}
			}
		}
	}
}

void ACapturePoint::CheckForFlagConstruction_Implementation()
{
	if (MainFlagCreator == true) {
		UE_LOG(LogTemp, Warning, TEXT("This is the main flag spot"));
		if (flagsCaptured >= requiredFlags) {
			
			if(mainFlag)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), flagSpawnEffect, this->GetActorLocation());

				GetWorldTimerManager().SetTimer(mainFlagActiveTimer, this, &ACapturePoint::SetMainFlagActive, flagInactivePeriod);
			}
			//main flag spawns on top of capture point
			
		}
	}
}

void ACapturePoint::RoundReset_Implementation()
{
	flagsCaptured = 0;
	
	if(MainFlagCreator && !IsValid(mainFlag))
	{
		FVector spawnPoint;
		spawnPoint = this->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
		mainFlag = Cast<AMainFlag>(GetWorld()->SpawnActor(AMainFlag::StaticClass(), &spawnPoint));
		
	}
	else if (MainFlagCreator && (mainFlag != nullptr))	{
		
		mainFlag->SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 60.0f));
	}
	if(mainFlag)
	{
		mainFlag->Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		Cast<USkeletalMeshComponent>(mainFlag->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetVisibility(false);
	}
	
}

void ACapturePoint::SetMainFlagActive_Implementation()	{
	mainFlag->CompleteMainFlag();
	//mainFlag->SetActorEnableCollision(true);
	//mainFlag->Capsule->SetCollisionResponseToAllChannels(ECR_Overlap);
	//Cast<USkeletalMeshComponent>(mainFlag->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetVisibility(true);
}

