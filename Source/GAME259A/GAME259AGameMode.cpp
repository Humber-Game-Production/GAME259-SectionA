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

	//make sure GM can tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);

	timerTime = 5.f;
}

void AGAME259AGameMode::BeginPlay() {
	Super::BeginPlay();
	
	//create timer
	GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, timerTime);
}

void AGAME259AGameMode::Tick(const float deltaTime) {
	Super::Tick(deltaTime);

	//add timer to screen
	if (GetWorldTimerManager().TimerExists(timerHandle) && GetWorldTimerManager().IsTimerPaused(timerHandle) == false) {
		timeLeft = FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(timerHandle));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, timeLeft);
	}
}

void AGAME259AGameMode::EndRound() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round over."));
}

void AGAME259AGameMode::SubtractTime() {
	if (GetWorldTimerManager().TimerExists(timerHandle)) {
		GetWorldTimerManager().SetTimer(timerHandle, this, &AGAME259AGameMode::EndRound, GetWorldTimerManager().GetTimerRemaining(timerHandle) - 1);
	}

	
}
