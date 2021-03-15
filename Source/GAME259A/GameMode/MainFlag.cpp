// Fill out your copyright notice in the Description page of Project Settings.


#include "MainFlag.h"
#include "GAME259A/Public/CTFPlayerState.h"



FTimerHandle FlagReserveTimer;

void AMainFlag::PickUp_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//checks if the OtherActor is a pawn
	APawn* isPawn = Cast<APawn>(OtherActor);
	if (isPawn) {
		//if it is, it checks if it has a playerState
		ACTFPlayerState* hasPlayerState = isPawn->GetPlayerState<ACTFPlayerState>();
		if(hasPlayerState)	{
			//check if Friendly has been defined, if not, assign one 
			if((owningTeam == ETeamIdentifier::None) || (owningTeam == hasPlayerState->teamID))	{

				AFlag::PickUp_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
			} else
			{
				if (GEngine) GEngine->UEngine::AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, TEXT("Flag is reserved for Enemy"));
			}
		}
	}
}

void AMainFlag::Drop_Implementation()
{
	AFlag::Drop_Implementation();
	GetWorldTimerManager().SetTimer(FlagReserveTimer,this,&AMainFlag::ReserveFlag,5.0f);
	
}

void AMainFlag::ReserveFlag(){
	owningTeam = ETeamIdentifier::None;
	AFlag::ChangeColour();
}
