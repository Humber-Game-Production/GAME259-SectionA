// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTFGameMode.generated.h"

UCLASS()
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

	UFUNCTION()
		void SubtractTime();

	//Change later to return a team
	UFUNCTION()
		bool WinCheck();

	UPROPERTY(EditAnywhere, Category = "Game Rules")
		float timerTime;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
		FString timeLeft;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
		int maxRounds;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
		int currentRound;

	//Place holder for team1 points
	UPROPERTY(EditAnywhere)
		int team1Points;

	//Place holder for team2 points
	UPROPERTY(EditAnywhere)
		int team2Points;

	FTimerHandle timerHandle;
};
