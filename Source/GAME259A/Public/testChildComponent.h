// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "testComponent.h"
#include "testChildComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAME259A_API UtestChildComponent : public UtestComponent
{
	GENERATED_BODY()

		void testFunction_Implementation(FVector location, float number) override;
	
};
