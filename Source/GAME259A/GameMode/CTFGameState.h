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

public:

	ACTFGameState();

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<ETeamIdentifier, ATeam*> listOfTeams;
	
	UFUNCTION()
	void ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player);

	UFUNCTION()
	void PlayerLeft(AController* player);
	
	UFUNCTION()
	void AddPoints(ETeamIdentifier team, int32 points);
	
	UFUNCTION()
	void InitTeams();
};
