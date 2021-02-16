// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Team.generated.h"

/**
 * 
 */
UENUM()
enum class PlayerType
{
	Human = 0 UMETA(DisplayName = "Human"),
	Alien = 1 UMETA(DisplayName = "Alien")
};

UCLASS(BlueprintType)
class GAME259A_API ATeam : public AActor
{
	GENERATED_BODY()

public:

	ATeam();

	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Team | Player")
	PlayerType playerType;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Player")
	TArray<AActor*> players;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Team | Places")
	TArray<AActor*> respawnPoints;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Team | Places")
	AActor* capturePoint;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 points;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Team | Info")
	int32 miniFlagsColllected;


	
	UFUNCTION(BlueprintCallable)
	void AddPlayer(AActor* player_);

	UFUNCTION(BlueprintCallable)
	void RemovePlayer(AActor* player_);

	UFUNCTION(BlueprintCallable)
	void AddPoints(int32 value);

	UFUNCTION(BlueprintCallable)
	void SpawnPlayers();

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer(AActor* player);
	
};
