// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePoint.h"


#include "CTFGameMode.h"
#include "CTFGameState.h"
#include "CTFPlayerState.h"
#include "GAME259A/GameMode/Team.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "MainFlag.h"
#include "MiniFlag.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

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
		mainFlag->SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
	}
	
	if(HasAuthority())
	{
		if(ACTFGameMode* ctfGameMode = GetWorld()->GetAuthGameMode<ACTFGameMode>())
		{
			requiredFlags = ctfGameMode->requiredMiniFlags;
		}

		if(ACTFGameState* ctfGameState = GetWorld()->GetGameState<ACTFGameState>())
		{
			ctfGameState->roundStartDelegate.AddDynamic(this, &ACapturePoint::SetCapActive);
			ctfGameState->roundEndDelegate.AddDynamic(this, &ACapturePoint::SetCapInactive);
			ctfGameState->gameEndDelegate.AddDynamic(this, &ACapturePoint::SetCapInactiveEnd);
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
	DOREPLIFETIME(ACapturePoint, capturePointActive);
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

void ACapturePoint::SetCapInactiveEnd(ETeamIdentifier team)
{
	capturePointActive = false;
}


void ACapturePoint::SetCapActive()
{
	capturePointActive = true;
}

void ACapturePoint::SetCapInactive()
{
	capturePointActive = false;
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
	if(capturePointActive)
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
						CheckForFlagConstruction();
						player->FlagHeld->InitLocation = FVector(0, -1000, 0);
						player->CaptureFlag();
						UE_LOG(LogTemp, Warning, TEXT("MiniFlag number %d captured at Midpoint"), flagsCaptured);
					
						//Main Flag building in here
						if(HasAuthority())
						{
							TestFunction();
							ACTFGameState* gameState = GetWorld()->GetGameState<ACTFGameState>();
							gameState->capturedFlags = flagsCaptured;
							gameState->CapturedFlagDelegate.Broadcast(flagsCaptured);
						}
					
					} //if the player's holding a main flag, this capture point is part of the same team as the player, and this is not the midPoint
					else if(Cast<AMainFlag>(player->FlagHeld) && (playersTeam == teamID) && (MainFlagCreator == false))	{
						player->CaptureFlag();
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), teamWinEffect, this->GetActorLocation());
						UE_LOG(LogTemp, Warning, TEXT("MainFlag captured at team %d's capture point"), teamID);
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
				GetWorldTimerManager().SetTimer(mainFlagActiveTimer, this, &ACapturePoint::SetMainFlagActive, flagInactivePeriod);
			}
			//main flag spawns on top of capture point
			
		}
	}
}

void ACapturePoint::RoundReset_Implementation()
{
	flagsCaptured = 0;

	if(HasAuthority())
	{
		ACTFGameState* gameState = GetWorld()->GetGameState<ACTFGameState>();
		if (gameState->activeFlags.Contains(mainFlag))
		{
			gameState->activeFlags.Remove(mainFlag);
		}
	}
	
	if(MainFlagCreator && !IsValid(mainFlag))
	{
		if(HasAuthority())
		{
			const auto& mainFlagType = GetWorld()->GetAuthGameMode<ACTFGameMode>()->mainFlag;
			FVector spawnPoint;
			spawnPoint = this->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
			mainFlag = Cast<AMainFlag>(GetWorld()->SpawnActor(mainFlagType, &spawnPoint));
		}
	}
	else if (MainFlagCreator && (mainFlag != nullptr))	{
		
		mainFlag->SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
	}
	
	if(mainFlag)	{
		mainFlag->ResetMainFlag();	
	}
	
}

void ACapturePoint::SetMainFlagActive_Implementation()	{
	mainFlag->CompleteMainFlag();
	if (mainFlag){
		if (HasAuthority())
		{
			ACTFGameState* gameState = GetWorld()->GetGameState<ACTFGameState>();
			gameState->activeFlags.Add(mainFlag);
			gameState->activeFlagsChangedDelegate.Broadcast(gameState->activeFlags);
		}
	}
}

