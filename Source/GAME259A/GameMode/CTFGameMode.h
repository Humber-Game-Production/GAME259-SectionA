// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CapturePoint.h"
#include "GameFramework/GameModeBase.h"
#include "TeamIdentifier.h"
#include "CTFGameMode.generated.h"

class ACTFGameState;

UCLASS(Blueprintable)
class GAME259A_API ACTFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACTFGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(const float deltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;



	UFUNCTION()
	void BeginFirstRound();

	void InitTeams();
	
	UFUNCTION()
	void SpawnMiniFlag();
	
	UFUNCTION()
	void EndRound();

	UFUNCTION()
	void RoundReset();

	//Change later to return a team
	UFUNCTION()
	bool WinCheck();

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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	FString timeLeft;

	//Total amount of rounds
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	int maxRounds;

	//Current round number
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	int currentRound;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CapturePoints")
	TArray<ACapturePoint*> capturePoints;
	
	TMap<ETeamIdentifier, const int*> teamPoints;

	FTimerHandle startGameTimer;
	FTimerHandle roundTimerHandle;
	FTimerHandle flagSpawnTimer;
	

	UPROPERTY(BlueprintReadWrite)
	ACTFGameState* ctfGameState;
};


