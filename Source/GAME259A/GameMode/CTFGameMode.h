// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TeamIdentifier.h"

#include "CTFGameMode.generated.h"

class ACTFGameState;
class ATeam;
class ACapturePoint;
class AFlag;
class ACTFPlayerState;

UCLASS(Blueprintable)
class GAME259A_API ACTFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACTFGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	


	UFUNCTION(BlueprintCallable)
	void BeginFirstRound();

	UFUNCTION(BlueprintCallable)
	void InitTeams();

	UFUNCTION(BlueprintCallable)
	void UpdateGameStateTime();
	
	UFUNCTION(BlueprintCallable)
	void SpawnMiniFlag();
	
	UFUNCTION(BlueprintCallable)
	void EndRound();

	UFUNCTION(BlueprintCallable)
	void RoundReset();

	//Change later to return a team
	UFUNCTION(BlueprintCallable)
	bool WinCheck();

	UFUNCTION(BlueprintCallable)
	void EndGame();

	//Adds points to this team
	UFUNCTION(Server, Reliable, BlueprintCallable)
    void AddPoints(ETeamIdentifier team, int32 value);

	//Spawns all the players on this team
	UFUNCTION(BlueprintCallable)
    void SpawnAllPlayersOnTeam(ETeamIdentifier team);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Flags")
	int32 spawnedMiniFlags;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Flags")
	int32 requiredMiniFlags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flags")
	TSubclassOf<AFlag> miniFlag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flags")
	TSubclassOf<AFlag> mainFlag;
	
	//Time left in the current round(float)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	float roundTimerTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	float timeBetweenFlagSpawns;
	
	//Time left in the current round(FString)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Game Rules")
	FString timeLeft;

	//Total amount of rounds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	int maxRounds;

	//Current round number
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Game Rules")
	int currentRound;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CapturePoints")
	TArray<ACapturePoint*> capturePoints;

	TArray<const int*> teamPoints;

	FTimerHandle startGameTimer;
	FTimerHandle roundTimerHandle;
	FTimerHandle flagSpawnTimer;
	

	UPROPERTY(BlueprintReadWrite)
	ACTFGameState* ctfGameState;
};


