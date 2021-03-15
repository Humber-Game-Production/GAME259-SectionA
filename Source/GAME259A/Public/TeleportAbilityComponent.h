// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbilityClass.h"
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
	//Overides ActivateAbility in BaseAbilityClass
		UFUNCTION(BlueprintCallable)
			void ActivateAbility_Implementation(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_) override;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
			UTeleportAbilityComponent* TeleportAbilityRef;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ActorToSpawn;

};
