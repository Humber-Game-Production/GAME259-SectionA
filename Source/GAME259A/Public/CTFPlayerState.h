// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "GAME259A/GameMode/Flag.h"

#include "CTFPlayerState.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddScoreToTeam, ETeamIdentifier, Team, int32, Score);
/**
 * 
 */
UCLASS(Blueprintable)
class GAME259A_API ACTFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ACTFPlayerState();

	//Identifier for what team the player is on
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ETeamIdentifier teamID;

	//How many points this player has earned
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 pointsEarned;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 kills;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 deaths;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 flagsCaptured;

	UPROPERTY()
	AFlag* FlagHeld;

	//This delegate is called whenever this player scores for their team
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FAddScoreToTeam teamScoreDelegate;

	//Sets the player's team
	UFUNCTION()
	void SetTeam(ETeamIdentifier team);

	UFUNCTION()
	void SetFlagHeld(AFlag* FlagHeld_);
	
	UFUNCTION(BlueprintCallable)
	void SetCanPickupFlag(bool PlayerCanPickupFlag_);

	UFUNCTION(BlueprintCallable)
	bool GetCanPickupFlag() const;
	
	//Adds score to the player's team using teamScoreDelegate
	UFUNCTION()
	void AddScore(int32 amountOfPoints);
	
	UFUNCTION()
	void ResetStats();

	UFUNCTION()
	void Death();

protected:
	

	//Player should only pickup flags if they are Alive and have no other flags
	UPROPERTY(BlueprintReadWrite)
	bool PlayerCanPickupFlag;
};

