// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GAME259A/GameMode/TeamIdentifier.h"
#include "CTFGameState.generated.h"

/**
 * 
 */

class AFlag;
class ATeam;
class ACapturePoint;
class ACTFPlayerState;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiFlagsDelegate, const TArray<AFlag*>&, flags);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSingleFlagDelegate, AFlag*, flag);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeamDelegate, ETeamIdentifier, team);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundEventDelegate);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIntegerDelegate, int32, num);

UCLASS(Blueprintable)
class GAME259A_API ACTFGameState : public AGameStateBase
{
	GENERATED_BODY()
	FTimerHandle timer;
public:

	ACTFGameState();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Map of teams
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<ATeam*> listOfTeams;

	UPROPERTY(ReplicatedUsing = OnRep_activeFlags, BlueprintReadWrite, VisibleAnywhere)
	TArray<AFlag*> activeFlags;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<ACapturePoint*> capturePoints;

	UPROPERTY(ReplicatedUsing = OnRep_capturedFlags, BlueprintReadWrite, VisibleAnywhere)
	int32 capturedFlags;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 requiredFlags;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 timeLeft;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 currentRound;
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	int32 maxRounds;

	UPROPERTY(BlueprintAssignable)
	FMultiFlagsDelegate activeFlagsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSingleFlagDelegate flagPickupDelegate;

	UPROPERTY(BlueprintAssignable)
	FSingleFlagDelegate flagDropDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FRoundEventDelegate roundEndDelegate;

	UPROPERTY(BlueprintAssignable)
	FRoundEventDelegate roundStartDelegate;

	UPROPERTY(BlueprintAssignable)
	FTeamDelegate gameEndDelegate;

	UPROPERTY(BlueprintAssignable)
	FIntegerDelegate CapturedFlagDelegate;
	
	//Adds a CTFPlayerState to the specified team
	UFUNCTION(BlueprintCallable)
	void ChooseTeam(ETeamIdentifier team, ACTFPlayerState* player);

	//Adds points to the specified team
	UFUNCTION(BlueprintCallable)
	void AddPoints(ETeamIdentifier team, int32 points);

	UFUNCTION(BlueprintCallable)
	ATeam* GetTeam(ETeamIdentifier team) const;
	
	UFUNCTION(NetMulticast, Reliable)
	void OnRep_activeFlags();

	UFUNCTION(NetMulticast, Reliable)
	void OnRep_capturedFlags();	

	UFUNCTION(NetMulticast, Reliable)
	void OnRoundStart();
	
	UFUNCTION(NetMulticast, Reliable)
	void OnRoundEnd();

	UFUNCTION(NetMulticast, Reliable)
	void OnGameEnd(ETeamIdentifier winningTeam);

	UFUNCTION(NetMulticast, Reliable)
    void FlagPickedUp(AFlag* flag);

	UFUNCTION(NetMulticast, Reliable)
	void FlagDropped(AFlag* flag);
};
