// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GAME259A/GameMode/TeamIdentifier.h"

#include "CTFPlayerState.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddScoreToTeam, ETeamIdentifier, Team, int32, Score);

class ACTFPlayerState;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRespawnPlayer, ETeamIdentifier, Team, ACTFPlayerState*, Player);

class AFlag;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlagDelegte, AFlag*, flag);

 
UCLASS(Blueprintable)
class GAME259A_API ACTFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ACTFPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Identifier for what team the player is on
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere)
	ETeamIdentifier teamID;

	//How many points this player has earned
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 pointsEarned;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 kills;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 deaths;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 flagsCaptured;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	AFlag* FlagHeld;

	//This delegate is called whenever this player scores for their team
	UPROPERTY(Replicated, BlueprintAssignable, Category = "EventDispatchers")
	FAddScoreToTeam teamScoreDelegate;

	UPROPERTY(Replicated, BlueprintAssignable, Category = "EventDispatchers")
	FRespawnPlayer respawnPlayerDelegate;

	UPROPERTY(Replicated, BlueprintAssignable, Category = "EventDispatchers")
	FFlagDelegte currentObjectiveDelegate;

	//Sets the player's team
	UFUNCTION()
	void SetTeam(ETeamIdentifier team);

	UFUNCTION()
	void SetFlagHeld(AFlag* FlagHeld_);
	
	UFUNCTION(BlueprintCallable)
	void SetCanPickupFlag(bool PlayerCanPickupFlag_);

	UFUNCTION(BlueprintCallable)
	bool GetCanPickupFlag() const;
	
	//Adds score to the player's team using teamScoreDelegate
	UFUNCTION()
	void AddScore(int32 amountOfPoints);
	
	UFUNCTION()
	void ResetStats();

	//Player drops flag intentionally. Will re-enable flag pickup
	UFUNCTION(Server, reliable)
	void PlayerDropFlag();

	UFUNCTION(Server, Reliable)
	void CaptureFlag();

	//Player will die and drop flag. Player cannot pickup new flags when dead.
	UFUNCTION(NetMulticast, Reliable)
	void OnDeath(ACharacter* character, float respawnTime);

	UFUNCTION(Server, Reliable)
	void OnRespawn();

	UFUNCTION()
	void UpdateObjective(AFlag* newFlag);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Abilities")
	float AbilityOneCooldownRemaining;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Abilities")
	float AbilityTwoCooldownRemaining;
	
	//Following is code to set the players animation state.
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Movement")
	bool bIsSprinting;					//True if the character is sprinting.
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Movement")
	bool bIsJumping;					//True if the character is jumping.

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "AnimControl")
	bool bIsSwinging;					//True if the player just input to melee attack.
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "AnimControl")
	bool bIsThrowing;					//True if the player just input to melee attack.
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "AnimControl")
	bool bIsDrawingBow;					//True if the player just input to melee attack.
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "animControl")
	bool bIsGrappleHook;
protected:

	//Player should only pickup flags if they are Alive and have no other flags
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool PlayerCanPickupFlag;
};

