// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "GAME259A/BaseCharacter.h"
#include "Team.generated.h"

/**
 * 
 */

class ACTFPlayerState;
class ACapturePoint;

UCLASS(BlueprintType)
class GAME259A_API ATeam : public AActor
{
	GENERATED_BODY()

public:

	ATeam();

	//The type of BaseCharacter class that's being used for this team
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Team | Player")
	TSubclassOf<ABaseCharacter> playerType;

	//Array of playerStates on this team
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Team | Player")
	TArray<ACTFPlayerState*> players;

	//Array of respawn points
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Team | Places")
	TArray<AActor*> respawnPoints;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Team | Places")
	TArray<AActor*> miniFlagSpawnPoints;
	
	//The capture point associated with this team
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Team | Places")
	ACapturePoint* capturePoint;

	//Points earned by this team
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 points;

	//How many miniflags this team has collected
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 miniFlagsCollected;

	//Identifier for this team 
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Team | Info")
	ETeamIdentifier teamID;

		

	UFUNCTION()
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Adds a player to this team
	UFUNCTION(BlueprintCallable)
	void AddPlayer(ACTFPlayerState* player_);

	//Removes a player from this team
	UFUNCTION(BlueprintCallable)
	void RemovePlayer(ACTFPlayerState* player_);

	//Adds points to this team
	UFUNCTION(BlueprintCallable)
	void AddPoints(int32 value);

private:
	HIDE_ACTOR_TRANSFORM_FUNCTIONS();
	
};
