// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAbilityClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	UFUNCTION(BlueprintNativeEvent)
	void ActivateAbility(FVector Location_, float CoolDown_, FRotator Rotation_);

	UFUNCTION(BlueprintCallable)
		void UseAbility(FVector Location_, float CoolDown_, FRotator Rotation_);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CoolDown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LastUsed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Damage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector location;
};
