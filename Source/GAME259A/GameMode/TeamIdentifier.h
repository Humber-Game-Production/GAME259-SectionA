// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "TeamIdentifier.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETeamIdentifier : uint8
{
	None  = 0 UMETA(DisplayName = "None"),
	Human = 1 UMETA(DisplayName = "Human"),
	Alien = 2 UMETA(DisplayName = "Alien"),
};
