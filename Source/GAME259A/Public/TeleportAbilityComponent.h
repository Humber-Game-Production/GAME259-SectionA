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

		UFUNCTION(BlueprintCallable)
			void SpawnActor(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_);

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
			UTeleportAbilityComponent* TeleportAbilityRef;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ActorToSpawn;

};
