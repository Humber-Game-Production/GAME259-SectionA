// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFlag::AFlag() : isHeld(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. Not needed rn
	//PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;


	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Root);

	
}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( AFlag, owningTeam);
	DOREPLIFETIME( AFlag, isHeld);
}

void AFlag::ChangeColour_Implementation()
{
	if(matInstance)
	{
		switch(owningTeam)
		{
		case ETeamIdentifier::None:
			matInstance->SetVectorParameterValue(FName("Color"), noTeamColour.baseColour);
			matInstance->SetVectorParameterValue(FName("Ecolor"), noTeamColour.emissiveColour);
			break;
		case ETeamIdentifier::Human:
			matInstance->SetVectorParameterValue(FName("Color"), humanTeamColour.baseColour);
			matInstance->SetVectorParameterValue(FName("Ecolor"), humanTeamColour.emissiveColour);
			break;
		case ETeamIdentifier::Alien:
			matInstance->SetVectorParameterValue(FName("Color"), alienTeamColour.baseColour);
			matInstance->SetVectorParameterValue(FName("Ecolor"), alienTeamColour.emissiveColour);
			break;
		default:
			break;
		}
	}
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

		//if it does and player CAN pickup flag, pickup
		if (hasPlayerState && hasPlayerState->GetCanPickupFlag() && isHeld==false) {
			isHeld = true;
			USceneComponent* PlayerMesh = OtherActor->FindComponentByClass<USkeletalMeshComponent>();

			owningTeam = hasPlayerState->teamID;
			if(HasAuthority())
			{
				ChangeColour();
			}
			
			//attach the flag to the socket "FlagHolder" on the character mesh
			this->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, (TEXT("FlagHolder")));

			hasPlayerState->SetFlagHeld(this);
			//player can only have 1 flag
			hasPlayerState->SetCanPickupFlag(false);
		}
	}
}

void AFlag::Drop_Implementation()	{
	//print message
	if(GEngine)	GEngine->UEngine::AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, TEXT("Flag Dropped"));
	this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepRelative,EDetachmentRule::KeepRelative,false));

	FHitResult Hit;
	const FVector Start = this->GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 2500.0f);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 10.0f, 0, 2.0f);
	
	if(Hit.IsValidBlockingHit())	{
		FVector NewLocation(Hit.ImpactPoint + DistanceFromGround);
		this->SetActorLocation(NewLocation, false);
	}
	else	{
		this->SetActorLocation(InitLocation, false);
	}
	isHeld = false;
}

void AFlag::OnRep_owningTeam()
{
	ChangeColour();
}

void AFlag::Capture_Implementation()	{
	Drop_Implementation();
}


