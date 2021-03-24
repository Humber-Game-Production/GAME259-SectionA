// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTFPlayerState.h"
#include "TeleportAbilityActor.h"
#include "BaseAbilityClass.generated.h"


UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class GAME259A_API UBaseAbilityClass : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseAbilityClass();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	//UFUNCTION(BlueprintNativeEvent)
	//void ActivateAbility(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_, FVector Velocity_);

	UFUNCTION(BlueprintCallable)
		void UseAbility(float CoolDown_, FTransform Transform_, float Duration_, ETeamIdentifier Team_, float Damage_, FVector Velocity_);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CoolDown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LastUsed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Damage;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
		//FVector location;

	//UPROPERTY()
		//UWorld* GetWorld(AActor* actor);


	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ActorToSpawn;
};
