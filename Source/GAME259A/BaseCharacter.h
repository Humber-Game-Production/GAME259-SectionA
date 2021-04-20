#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Public/BaseAbilityClass.h"


#include "BaseCharacter.generated.h"

UCLASS()
class GAME259A_API ABaseCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();


	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;	//The boom stick for the camera. Controls how far away the camera is from the character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* ThirdPersonCamera;//The camera that will follow the character.

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float MaxWalkSpeed;					//The maximum speed at which the character can move while not sprinting.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	float CurrentMoveSpeed;				//The character's current speed at which they can walk. This can change.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float SprintMultiplier;				//The multiplier for the character's speed when sprinting.
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float JumpVelocity;					//The velocity at which the character will jump.

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Throwing")
	float MovementThrowLength;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Throwing")
	float MovementThrowHeight;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Throwing")
	float SmokeThrowLength;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Throwing")
	float SmokeThrowHeight;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth;					//The character's maximum health. CurrentHealth will be set to this value on initialization and if the value ever exceeds this.
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Health")
	float CurrentHealth;				//The character's current health.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Death")
	float RespawnTime;					//The amount of time required for the character to respawn.
	
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	float AbilityOneCoolDown;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	float AbilityTwoCoolDown;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	bool CanUseAbilityOne;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	bool CanUseAbilityTwo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CCStatus")
	bool bIsDead;						//True if the character is dead.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CCStatus")
	bool bIsSlowed;						//True if the character is slowed via crowd control.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CCStatus")
	float SlowMultiplier;				//The multiplier by which a character is slowed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CCStatus")
	bool bIsStunned;					//True if the character is stunned via crowd control.
	
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	UBaseAbilityClass* MovementAbility;//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	UBaseAbilityClass* SecondAbility;
	UPROPERTY()
	FTransform location;

	UPROPERTY(BlueprintReadWrite, Category = "AnimControl")
	bool bIsSwinging;					//True if the player just input to melee attack.
	
	//Jump timer handle
	FTimerHandle JumpTimer;
	//Handle to manage the respawn timer.
	FTimerHandle RespawnTimerHandle;
	//Handle to manage the throwing animation timer.
	FTimerHandle ThrowingTimer;
	//Handle to manage ability cooldowns
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	FTimerHandle AbilityOneTimerHandle;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	FTimerHandle AbilityTwoTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Death")
	UNiagaraComponent* respawnEffect;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void MeleeSwing_Implementation(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

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
	UFUNCTION(Category = "Abilities", Server, Reliable)
	void UseAbilityOne();
	UFUNCTION(Category = "Abilities", Server, Reliable)
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
	UFUNCTION(Server, Reliable, Category = "Death", BlueprintCallable)
	void Respawn();

	UFUNCTION(Server, Reliable)
	void SetThrowAbilityOne();
	UFUNCTION(Server, Reliable)
	void SetThrowAbilityTwo();

	//Functions to send animation booleans to server.
	UFUNCTION(Server, Reliable)
	void SetIsSprinting(bool bIsSprinting);
	UFUNCTION(Server, Reliable)
	void SetIsJumping(bool bIsJumping);
	UFUNCTION(Server, Reliable)
	void SetIsSwinging(bool bIsSwinging_);
	UFUNCTION(Server, Reliable)
	void SetIsThrowing(bool bIsThrowing);
	UFUNCTION(Server, Reliable)
	void SetIsDrawingBow(bool bIsDrawingBow);

	//UPROPERTY(BlueprintReadOnly, Replicated, Category = "Movement")
	//bool bIsSprinting;					//True if the character is sprinting.
	//UPROPERTY(BlueprintReadOnly, Replicated, Category = "Movement")
	//bool bIsJumping;					//True if the character is jumping.
	
	//UPROPERTY(BlueprintReadWrite, Replicated, Category = "AnimControl")
	//bool bIsThrowing;					//True if the player just input to melee attack.
	//UPROPERTY(BlueprintReadWrite, Replicated, Category = "AnimControl")
	//bool bIsDrawingBow;					//True if the player just input to melee attack.
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Category = "Abilities", BlueprintCallable)
		float GetAbilityOneCooldown();
	UFUNCTION(Category = "Abilities", BlueprintCallable)
		float GetAbilityTwoCooldown();

	UPROPERTY(EditAnywhere)
		UBoxComponent* MeleeBox;

	UFUNCTION(Category = "Movement", BlueprintCallable)
	void Slow();
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void UnSlow();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Movement")
	bool bRecentlyLaunched;					//True if the character is stunned via crowd control.
};
