// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbilityClass.h"
#include "SplineEndPointActor.h"
#include "Components/SplineComponent.h"
#include "../BaseCharacter.h"

#include "TeleportAbilityComponent.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GAME259A_API UTeleportAbilityComponent : public UBaseAbilityClass
{
	GENERATED_BODY()
public:



		UFUNCTION(BlueprintCallable)
			void SpawnActor(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, FVector Velocity_);

		UFUNCTION(BlueprintCallable, Category = "Throwing")
			FVector GetVelocity(FVector Velocity_);

		UFUNCTION(BlueprintCallable, Category = "Throwing")
			void CreatePredictionSpline();

		UFUNCTION(BlueprintCallable)
			void StartCoolDown(float CoolDown_);

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
			UTeleportAbilityComponent* TeleportAbilityRef;
		UPROPERTY(BlueprintReadOnly)
			bool CanUseAbility;

		UPROPERTY()
			FVector Vel;

protected:
	UTeleportAbilityComponent();
private:

	FTimerHandle AbilityTimerHandle;
	float AbilityCoolDownRemaining;
	
	void ResetAbilityCoolDown();



	UPROPERTY()
		bool isAiming = true;
	UPROPERTY()
		ASplineEndPointActor* EndPoint;
	UPROPERTY()
		USplineComponent* PredictionSpline;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ActorToSpawn;

};
