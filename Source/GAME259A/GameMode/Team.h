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

UCLASS(BlueprintType)
class GAME259A_API ATeam : public AActor
{
	GENERATED_BODY()

public:

	ATeam();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team | Player")
	TSubclassOf<ABaseCharacter> playerType;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Player")
	TArray<ACTFPlayerState*> players;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Team | Places")
	TArray<AActor*> respawnPoints;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Team | Places")
	AActor* capturePoint;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 points;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 miniFlagsColllected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team | Info")
	ETeamIdentifier teamID;
	
	UFUNCTION(BlueprintCallable)
	void AddPlayer(ACTFPlayerState* player_);

	UFUNCTION(BlueprintCallable)
	void RemovePlayer(ACTFPlayerState* player_);

	UFUNCTION(BlueprintCallable)
	void AddPoints(int32 value);

	UFUNCTION(BlueprintCallable)
	void SpawnPlayers();

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer(AActor* player);

private:
	HIDE_ACTOR_TRANSFORM_FUNCTIONS();
	
};
