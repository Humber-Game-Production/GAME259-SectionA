// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLobbyPlayerInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Info")
		bool bPlayerReadyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Info")
		FString PlayerName;
};

UCLASS()
class GAME259A_API ALobbyGameMode : public AGameModeBase
{
public:
	GENERATED_BODY()


		TArray<struct FLobbyPlayerInfo> PlayerInfoArray;

	UPROPERTY(EditDefaultsOnly)
		FString GameMapName;

public:

	TArray<class ALobbyPlayerController*> ConnectedPlayers;

	UPROPERTY(BlueprintType);
	TArray<class ALobbyGameMode*> AllPlayerControllers;

	//called when a new player joins the game
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* ExitingPlayer) override;

	//called from player controller when he sends a chat message
	void ProdcastChatMessage(const FText& ChatMessage);

	//called from the host to kick a player
	void KickPlayer(int32 PlayerIndex);

	void PlayerRequestUpdate();

	void UpdatePlayerList();

	UFUNCTION(BlueprintCallable, Category = "Network|Lobby")
	void StartGameFromLobby();

	bool IsAllPlayerReady() const;

	UFUNCTION(BlueprintPure, Category = "Network|Lobby")
		bool CanWeStartGame() const;
};