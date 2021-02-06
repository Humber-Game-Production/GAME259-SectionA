// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME259AGameMode.h"
#include "GAME259ACharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAME259AGameMode::AGAME259AGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Core/Characters/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
