// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbilityClass.h"
#include "TeleportAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAME259A_API UTeleportAbilityComponent : public UBaseAbilityClass
{
	GENERATED_BODY()

		void ActivateAbility_Implementation(FVector Location_, float CoolDown_) override;
	
};
