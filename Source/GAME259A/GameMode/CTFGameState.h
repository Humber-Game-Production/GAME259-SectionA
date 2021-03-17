// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GAME259A/GameMode/Team.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "CTFGameState.generated.h"

/**
 * 
 */
UCLASS()
class GAME259A_API ACTFGameState : public AGameStateBase
{
	GENERATED_BODY()
	FTimerHandle timer;
public:

	ACTFGameState();

	virtual void BeginPlay() override;

	//Map of teams
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<ETeamIdentifier, ATeam*> listOfTeams;

	//Adds a CTFPlayerState to the specified team
	UFUNCTION()
	void ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player);

	//This should be called whenever a player leaves a team or the server
	//@TODO: Implement this code
	UFUNCTION()
	void PlayerLeft(AController* player);

	//Adds points to the specified team
	UFUNCTION()
	void AddPoints(ETeamIdentifier team, int32 points);

	//Initializes the team's variables
	//@TODO: This function needs to be called whenever all players are loaded in
	UFUNCTION()
	void InitTeams();
};
