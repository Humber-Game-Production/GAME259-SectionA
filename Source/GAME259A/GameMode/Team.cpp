// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include "GAME259A/BaseCharacter.h"
#include "GAME259A/Public/CTFPlayerState.h"
#include "GAME259A/GameMode/CapturePoint.h"

ATeam::ATeam()
{
	teamID = ETeamIdentifier::Human;
	capturePoint = nullptr;
	points = 0;
	miniFlagsColllected = 0;
}

void ATeam::BeginPlay()
{
	if(capturePoint)
	{
		capturePoint->teamID = teamID;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Team %d's capture point is nullptr"), teamID);
	}
	SpawnPlayers();
}

void ATeam::AddPlayer(ACTFPlayerState* player_)
{
	player_->SetTeam(teamID);
	players.Add(player_);
	
}

void ATeam::RemovePlayer(ACTFPlayerState* player_)
{
	players.Remove(player_);
}

void ATeam::AddPoints(int32 value)
{
	points += value;
	UE_LOG(LogTemp, Warning, TEXT("Team %d has %d points"), teamID, points);
}

void ATeam::SpawnPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning all players"));
	for(int i = 0; i < players.Num(); i++)
	{
		APawn* pawn = players[i]->GetPawn();
		players[i]->OnDeath();
		SpawnPlayer(players[i]->GetPawn());
		pawn->Destroy();
	}
}

void ATeam::SpawnPlayer(APawn* pawn)
{
	const FVector location = respawnPoints[0]->GetActorLocation();
	const FRotator rotation = respawnPoints[0]->GetActorRotation();
	
	AActor* player = GetWorld()->SpawnActor(playerType, &location, &rotation);
	APawn* newPawn = Cast<APawn>(player);
	AController* controller = pawn->GetController();
	
	controller->UnPossess();
	newPawn->SetPlayerState(pawn->GetPlayerState());
	controller->Possess(newPawn);

	newPawn->GetPlayerState<ACTFPlayerState>()->SetCanPickupFlag(true);
}
