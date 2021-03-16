// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "Team.generated.h"

/**
 * 
 */

class ABaseCharacter;
class ACTFPlayerState;
class ACapturePoint;

UCLASS(BlueprintType)
class GAME259A_API ATeam : public AActor
{
	GENERATED_BODY()

public:

	ATeam();

	//The type of BaseCharacter class that's being used for this team
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team | Player")
	TSubclassOf<ABaseCharacter> playerType;

	//Array of playerStates on this team
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Player")
	TArray<ACTFPlayerState*> players;

	//Array of respawn points
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team | Places")
	TArray<AActor*> respawnPoints;

	//The capture point associated with this team
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team | Places")
	ACapturePoint* capturePoint;

	//Points earned by this team
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 points;

	//How many miniflags this team has collected
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 miniFlagsColllected;

	//Identifier for this team 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team | Info")
	ETeamIdentifier teamID;

		

	UFUNCTION()
	virtual void BeginPlay() override;

	//Adds a player to this team
	UFUNCTION(BlueprintCallable)
	void AddPlayer(ACTFPlayerState* player_);

	//Removes a player from this team
	UFUNCTION(BlueprintCallable)
	void RemovePlayer(ACTFPlayerState* player_);

	//Adds points to this team
	UFUNCTION(BlueprintCallable)
	void AddPoints(int32 value);

	//Spawns all the players on this team
	UFUNCTION(BlueprintCallable)
	void SpawnPlayers();

	//Spawns a single player on this team
	UFUNCTION(BlueprintCallable)
	void SpawnPlayer(APawn* pawn);

private:
	HIDE_ACTOR_TRANSFORM_FUNCTIONS();
	
};
