// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PickUpAndDrop.h"
#include "TeamIdentifier.h"
#include "Flag.generated.h"

USTRUCT(BlueprintType)
struct FUColourPair
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor baseColour = FLinearColor::Gray;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor emissiveColour = FLinearColor::Gray;
};

UCLASS(BlueprintType)
class GAME259A_API AFlag : public AActor, public IPickUpAndDrop
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFlag();

	UFUNCTION()
	virtual void Capture();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ChangeColour();

	//Flag pickup when the capsule overlaps with the player(OtherActor)
	UFUNCTION()
	virtual void PickUp_Implementation(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	UFUNCTION()
	virtual void Drop_Implementation() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeamIdentifier owningTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInstanceDynamic* matInstance;
public:	
	
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 pointValue;

	UPROPERTY(BlueprintReadOnly)
	FVector InitLocation;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector DistanceFromGround;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FUColourPair noTeamColour;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FUColourPair humanTeamColour;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FUColourPair alienTeamColour;
	
};
