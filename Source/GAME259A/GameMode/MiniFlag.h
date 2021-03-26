// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Flag.h"
#include "MiniFlag.generated.h"

/**
 * 
 */
UCLASS()
class GAME259A_API AMiniFlag : public AFlag
{
	GENERATED_BODY()

protected:

	virtual void Capture() override;
};
