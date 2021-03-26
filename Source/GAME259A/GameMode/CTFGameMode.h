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

UCLASS(Blueprintable)
class GAME259A_API ACTFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACTFGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	


	UFUNCTION()
	void BeginFirstRound();

	UFUNCTION()
	void InitTeams();

	UFUNCTION()
	void UpdateGameStateTime();
	
	UFUNCTION()
	void SpawnMiniFlag();
	
	UFUNCTION()
	void EndRound();

	UFUNCTION()
	void RoundReset();

	//Returns winning team
	UFUNCTION()
	ETeamIdentifier WinCheck();

	UFUNCTION()
	ATeam* GetTeam(ETeamIdentifier team) const;

	//Adds points to this team
	UFUNCTION(Server, Reliable, BlueprintCallable)
    void AddPoints(ETeamIdentifier team, int32 value);

	//Spawns all the players on this team
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
    void SpawnAllPlayersOnTeam(ETeamIdentifier team);

	//Spawns a single player on this team
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
    void SpawnPlayer(APawn* pawn);

	
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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Game Rules")
	int32 maxPoints;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CapturePoints")
	TArray<ACapturePoint*> capturePoints;

	TArray<const int*> teamPoints;

	FTimerHandle startGameTimer;
	FTimerHandle roundTimerHandle;
	FTimerHandle flagSpawnTimer;
	

	UPROPERTY(BlueprintReadWrite)
	ACTFGameState* ctfGameState;
};


