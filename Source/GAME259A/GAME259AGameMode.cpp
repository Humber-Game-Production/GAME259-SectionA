// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME259AGameMode.h"
#include "GAME259ACharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

AGAME259AGameMode::AGAME259AGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Core/Characters/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AGAME259AGameMode::BeginPlay() {
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, 1.f, false);
}

void AGAME259AGameMode::Tick(const float deltaTime) {
	Super::Tick(deltaTime);
}

void AGAME259AGameMode::EndRound() {
	
}
