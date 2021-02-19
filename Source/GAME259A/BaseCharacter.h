#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

#include "BaseCharacter.generated.h"

UCLASS()
class GAME259A_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;	//The boom stick for the camera. Controls how far away the camera is from the character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* ThirdPersonCamera;//The camera that will follow the character.

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MaxWalkSpeed;					//The maximum speed at which the character can move while not sprinting.
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentMoveSpeed;				//The character's current speed at which they can walk. This can change.
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float SprintMultiplier;				//The multiplier for the character's speed when sprinting.
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;					//True if the character is sprinting.
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float JumpVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth;					//The character's maximum health. CurrentHealth will be set to this value on initialization and if the value ever exceeds this.
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;				//The character's current health.
	
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	bool bIsDead;						//True if the character is dead.
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	bool bIsSlowed;						//True if the character is slowed via crowd control.
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	bool bIsStunned;					//True if the character is stunned via crowd control.

	float RespawnTime;					//The amount of time required for the character to respawn.

	//Handle to manage the respawn timer.
	FTimerHandle RespawnTimerHandle;

	//**THIS IS COMMENTED OUT UNTIL ABILITY BASE CLASS IS MADE**
	//UPROPERTY(BlueprintReadWrite, Category = "Abilities")
	//TArray<Ability> Abilities; //A list of abilities the character can use. This will be initialized to empty and filled by a blueprint that inherits from this class.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Category = "Movement")
	void MoveRight(float Axis);
	UFUNCTION(Category = "Movement")
	void MoveForward(float Axis);
	UFUNCTION(Category = "Death")
	void Death();
	UFUNCTION(Category = "Death")
	void Respawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
