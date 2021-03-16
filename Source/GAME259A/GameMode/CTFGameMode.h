// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	void EndRound();

	UFUNCTION()
	void RoundReset();

	//Change later to return a team
	UFUNCTION()
	bool WinCheck();

	//Time left in the current round(float)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	float timerTime;

	//Time left in the current round(FString)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	FString timeLeft;

	//Total amount of rounds
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	int maxRounds;

	//Current round number
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Rules")
	int currentRound;


	const int* humanPoints;
	const int* alienPoints;

	FTimerHandle timerHandle;

	UPROPERTY(BlueprintReadWrite)
	ACTFGameState* ctfGameState;
};
