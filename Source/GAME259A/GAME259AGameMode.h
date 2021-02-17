// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAME259AGameMode.generated.h"

UCLASS(minimalapi)
class AGAME259AGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAME259AGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(const float deltaTime) override;
	void EndRound();

	FTimerHandle timerHandle;
};



