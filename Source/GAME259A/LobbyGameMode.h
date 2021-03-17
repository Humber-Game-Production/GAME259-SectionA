// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAME259A_API ALobbyGameMode : public AGameModeBase
{
public:
	GENERATED_BODY()

	ALobbyGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(const float deltaTime) override;
	void ServerUpdateGameOptions(int MapID_, int TimeID_);

	TArray<class APlayerController*> PlayerControllerList;
	// Overriding the PostLogin function
	virtual void PostLogin(APlayerController* NewPlayer) override;


private:
	int MapID;
	int TimeID;

	// Maximum Number of Players needed/allowed during this Match
	int32 MaxNumPlayers;

	ALobbyGameMode* lobbyGameMode;
	/*PlayerInfo connectedPlayers[6];
	Texture2D* MapDisplay;*/
	FString MapTimeDisplay;
	FString MapNameDisplay;
};
