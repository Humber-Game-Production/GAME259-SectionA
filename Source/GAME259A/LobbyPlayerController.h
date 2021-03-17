// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
USTRUCT()
struct FPlayerInfo {

    GENERATED_USTRUCT_BODY()
    FString MyPlayerName;
    FString MyPlayerCharacterName;
	FString MyPlayerStatus;
};

UCLASS()
class GAME259A_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

		UPROPERTY(Replicated)
		int32 OurVariable;

	UFUNCTION(Server, unreliable, WithValidation)
		void Server_IncreaseVariable();

public:

	ALobbyPlayerController();

	// Also overriding the BeginPlay function for this example
	virtual void BeginPlay() override;

	// Function to Increment the Variable
	void IncreaseVariable();

private:
	FPlayerInfo* PlayerSettings;
	
};
