// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "CTFGameState.generated.h"

/**
 * 
 */

class AFlag;
class ATeam;
class ACapturePoint;
class ACTFPlayerState;

UCLASS(Blueprintable)
class GAME259A_API ACTFGameState : public AGameStateBase
{
	GENERATED_BODY()
	FTimerHandle timer;
public:

	ACTFGameState();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Map of teams
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<ATeam*> listOfTeams;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<AActor*> flagHolders;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<AActor*> activeFlags;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<ACapturePoint*> capturePoints;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 timeLeft;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 currentRound;
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 maxRounds;


	
	//Adds a CTFPlayerState to the specified team
	UFUNCTION()
	void ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player);

	//Adds points to the specified team
	UFUNCTION()
	void AddPoints(ETeamIdentifier team, int32 points);

	UFUNCTION()
	ATeam* GetTeam(ETeamIdentifier team) const;
};
