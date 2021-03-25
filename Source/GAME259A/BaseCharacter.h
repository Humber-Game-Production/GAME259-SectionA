#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Public/BaseAbilityClass.h"

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
	bool bIsJumping;					//True if the character is jumping.
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float JumpVelocity;					//The velocity at which the character will jump.

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth;					//The character's maximum health. CurrentHealth will be set to this value on initialization and if the value ever exceeds this.
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentHealth;				//The character's current health.
	UPROPERTY(BlueprintReadWrite, Category = "Death")
	float RespawnTime;					//The amount of time required for the character to respawn.
	
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	bool bIsDead;						//True if the character is dead.
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	bool bIsSlowed;						//True if the character is slowed via crowd control.
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	float SlowMultiplier;				//The multiplier by which a character is slowed
	UPROPERTY(BlueprintReadOnly, Category = "CCStatus")
	bool bIsStunned;					//True if the character is stunned via crowd control.
	UPROPERTY(BlueprintReadWrite, Category = "AnimControl")
	bool bIsSwinging;					//True if the player just input to melee attack.
	UPROPERTY(BlueprintReadWrite, Category = "AnimControl")
	bool bIsThrowing;					//True if the player just input to melee attack.
	UPROPERTY(BlueprintReadWrite, Category = "AnimControl")
	bool bIsDrawingBow;					//True if the player just input to melee attack.

	UPROPERTY(EditAnywhere, Category = "Abilities")
	UBaseAbilityClass* TeleportAbility;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	UBaseAbilityClass* SecondAbility;
	UPROPERTY()
	FTransform location;


	//Jump timer handle
	FTimerHandle JumpTimer;
	//Handle to manage the respawn timer.
	FTimerHandle RespawnTimerHandle;
	//Handle to manage the throwing animation timer.
	FTimerHandle ThrowingTimer;

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

	UFUNCTION(Category = "Movement", BlueprintCallable)
	void Walk(FVector Direction);
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void Sprint();
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void StopSprinting();
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void StartJump();

	//Abilities
	UFUNCTION(Category = "Abilities")
	void UseAbilityOne();
	UFUNCTION(Category = "Abilities")
	void UseAbilityTwo();
	UFUNCTION()
	void DropFlag();
	
	//Combat Actions
	UFUNCTION(Category = "Combat", BlueprintCallable)
	void UseMeleeAttack();
	UFUNCTION(Category = "Combat", BlueprintCallable)
	void UseRangedAttack();

	UFUNCTION(Category = "Death", BlueprintCallable)
	void TakeDamage(float damage_);
	UFUNCTION(NetMulticast, Reliable, Category = "Death", BlueprintCallable)
	void Death();
	UFUNCTION(NetMulticast, Reliable, Category = "Death", BlueprintCallable)
	void Respawn();

	UFUNCTION()
	void SetThrowAbilityOne();
	UFUNCTION()
	void SetThrowAbilityTwo();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Category = "Movement", BlueprintCallable)
	void Slow();
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void UnSlow();

};
