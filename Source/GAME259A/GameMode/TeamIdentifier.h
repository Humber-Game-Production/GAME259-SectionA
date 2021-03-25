// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "TeamIdentifier.generated.h"

/**
 * Identifiers for the teams
 */
UENUM(BlueprintType)
enum class ETeamIdentifier : uint8
{
	Human = 0 UMETA(DisplayName = "Human"),
	Alien = 1 UMETA(DisplayName = "Alien"),
	None  = 3 UMETA(DisplayName = "None")
};
