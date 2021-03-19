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

class AFlag;

UCLASS(Blueprintable)
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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<AActor*> flagHolders;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<AFlag*> activeFlags;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ACapturePoint*> capturePoints;
	
	//Adds a CTFPlayerState to the specified team
	UFUNCTION()
	void ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player);

	//Adds points to the specified team
	UFUNCTION()
	void AddPoints(ETeamIdentifier team, int32 points);
};
