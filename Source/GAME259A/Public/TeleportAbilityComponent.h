// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeleportAbilityActor.h"
#include "BaseAbilityClass.h"
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

		UFUNCTION()
		virtual void BeginPlay() override;

		//UFUNCTION()
			//void ActivateAbility_Implementation(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_, FVector Velocity_) override;

		UFUNCTION(BlueprintCallable, Category = "Throwing")
			FVector GetVelocity(FVector Velocity_);

		UFUNCTION(BlueprintCallable, Category = "Throwing")
			void CreatePredictionSpline();

		UFUNCTION(BlueprintCallable)
			void StartCoolDown(float CoolDown_);
		
		UPROPERTY(BlueprintReadOnly)
			bool CanUseAbility;

		UPROPERTY(BlueprintReadOnly)
			float AbilityCoolDownRemaining;

		UPROPERTY()
			FVector Vel;


		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		float MaxCoolDown;
protected:
	UTeleportAbilityComponent();
private:

	FTimerHandle AbilityTimerHandle;
	
	
	void ResetAbilityCoolDown();

	UPROPERTY()
		bool isAiming = true;


};
