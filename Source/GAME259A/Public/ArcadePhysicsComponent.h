// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "ArcadePhysicsComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME259A_API UArcadePhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArcadePhysicsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Varibles
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Defaults")
		float GravityStrenght = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
		float Mass = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
		bool bUseMass = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves")
		UCurveFloat* JumpCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves")
		UCurveFloat* ThrowCurve;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
