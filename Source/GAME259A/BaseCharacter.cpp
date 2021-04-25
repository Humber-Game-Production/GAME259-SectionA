#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "GameMode/Team.h"
#include "CTFPlayerState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter() : MaxWalkSpeed(1200.0f), SprintMultiplier(1.5f), JumpVelocity(800.0f), MovementThrowLength(1200.0f), MovementThrowHeight(500.0f), SmokeThrowLength(1200.0f), SmokeThrowHeight(500.0f),
MaxHealth(100.0f), CurrentHealth(MaxHealth), RespawnTime(3.0f), AbilityOneCoolDown(8.0f), AbilityTwoCoolDown(12.0f), CanUseAbilityOne(true), CanUseAbilityTwo(true), bIsDead(false), bIsSlowed(false),
SlowMultiplier(0.25f), bIsStunned(false), bRecentlyLaunched(false)
{
	this->Tags.Add(FName("Player"));
	
	//Set the character to not rotate when the mouse is moved, only the camera is rotated.
 	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	bIsAttacking = false;
	
	bIsDead = false;

	respawnEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RespawnEffectComponent"));
	respawnEffect->SetupAttachment(RootComponent);

	//Set collision capsule.
	Hurtbox = GetCapsuleComponent();
	Hurtbox->InitCapsuleSize(42.0f, 96.0f);

	MeleeSwingHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	MeleeSwingHitbox->SetupAttachment(GetMesh());

	//Set this so the character does not turn to look in the direction they are moving.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Set how fast they turn to look in the direction they are moving.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	GetCharacterMovement()->AirControl = 0.5f;

	//Setup camera arm. This controls how far away the camera is from the character.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	//Setup the camera and attach it to the camera arm.
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	MovementAbility = CreateDefaultSubobject<UBaseAbilityClass>(TEXT("MovementAbility"));
	SecondAbility = CreateDefaultSubobject<UBaseAbilityClass>(TEXT("SecondAbility"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	MeleeSwingHitbox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::MeleeSwing_Implementation);

	CurrentMoveSpeed = MaxWalkSpeed;
	//Set how fast the character jumps.
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	respawnEffect->ActivateSystem();
}

void ABaseCharacter::MeleeSwing_Implementation(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsAttacking = true;
	//check if overlap is pawn
	APawn* isPawn = Cast<APawn>(OtherActor);
	

	if (isPawn) {
		//if it is, it checks if it has a playerState
		ACTFPlayerState* hasPlayerState = isPawn->GetPlayerState<ACTFPlayerState>();
		ACTFPlayerState* myPlayerState = this->GetPlayerState<ACTFPlayerState>();

		if (hasPlayerState && myPlayerState) {
			if (myPlayerState->teamID != hasPlayerState->teamID) {
				UCapsuleComponent* enemyHurtbox = isPawn->FindComponentByClass<UCapsuleComponent>();

				if (enemyHurtbox) {
					if (MeleeSwingHitbox->IsOverlappingComponent(enemyHurtbox)) {
						ABaseCharacter* enemyChar = Cast<ABaseCharacter>(isPawn);
						enemyChar->TakeDamage(20.0f);
					}

				}
			}
		}
	}

	bIsAttacking = false;
}

//Called when the player is supposed to move left (Axis = -1) or right (Axis = 1).
void ABaseCharacter::MoveRight(float Axis)
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	//FVector right = GetActorRightVector();

	//if (Axis < 0) right *= -1;
	//if (Axis == 0) right *= 0;
	if (bIsSlowed) {
		Axis = Axis * SlowMultiplier;
	}
	if (ctfPlayerState) {
		if (!ctfPlayerState->bIsSprinting)	{
			Axis = Axis * 1 / SprintMultiplier;
		}
	}
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);

	
	//Walk(right);
}


void ABaseCharacter::Walk(FVector Direction) {

	AddMovementInput(Direction, CurrentMoveSpeed);
}

void ABaseCharacter::Sprint() {
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	//CurrentMoveSpeed = MaxWalkSpeed * SprintMultiplier;
	//GetCharacterMovement()->MaxWalkSpeed = CurrentMoveSpeed;
	if(ctfPlayerState)	{
		ctfPlayerState->bIsSprinting = true;
		SetIsSprinting(ctfPlayerState->bIsSprinting);
	}
	
}

void ABaseCharacter::StopSprinting()
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
//	CurrentMoveSpeed = MaxWalkSpeed;
	//GetCharacterMovement()->MaxWalkSpeed = CurrentMoveSpeed;
	if (ctfPlayerState) {
		ctfPlayerState->bIsSprinting = false;
		SetIsSprinting(ctfPlayerState->bIsSprinting);
	}
}

//Called when the "Jump" input is pressed. 
void ABaseCharacter::StartJump()
{
	if (!this) {
		return;
	}
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if (ctfPlayerState) {
		if (!GetCharacterMovement()->IsFalling() && !GetWorld()->GetTimerManager().IsTimerActive(JumpTimer)) {
			ctfPlayerState->bIsJumping = true;
			SetIsJumping(ctfPlayerState->bIsJumping);
			GetWorld()->GetTimerManager().SetTimer(JumpTimer, [this]()	{
					if (this)	{
						ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
						if (ctfPlayerState)	{
							Jump();
							ctfPlayerState->bIsJumping = false;
							SetIsJumping(ctfPlayerState->bIsJumping);
						}
					}
				},
				0.00001f, false);
		}
	}
}

//Called when the player is supposed to move forward (Axis = 1) or backward (Axis = -1).
void ABaseCharacter::MoveForward(float Axis)
{
	if (!this) {
		return;
	}
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	
	//FVector forward = GetActorForwardVector();
	//if (Axis < 0) forward *= -1;
	//if (Axis == 0) forward *= 0;
	if (bIsSlowed) {
		Axis = Axis * SlowMultiplier;
	}
	if (ctfPlayerState != nullptr) {
		if (!ctfPlayerState->bIsSprinting)
		{
			Axis = Axis * 1 / SprintMultiplier;
		}
	}
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
	//Walk(forward);
}

void ABaseCharacter::SetThrowAbilityOne_Implementation()
{
	if (!this) {
		return;
	}
	if (!bIsDead) {
		ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();

		//FVector tmpLoc = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100);
		//location = FTransform(tmpLoc + GetActorRightVector() * 40.0f + GetActorForwardVector() * 40.0f);
		location = FTransform(ThirdPersonCamera->GetRelativeLocation() + CameraBoom->GetRelativeLocation() + RootComponent->GetRelativeLocation() + ThirdPersonCamera->GetForwardVector() * 80.0f);
		FVector ThrowDistance = ThirdPersonCamera->GetForwardVector() * MovementThrowLength + ThirdPersonCamera->GetUpVector() * MovementThrowHeight;
		if (ACTFPlayerState * StateOfPlayer = GetPlayerState<ACTFPlayerState>())
			MovementAbility->UseAbility(3.0f, location, 0.0f, StateOfPlayer->teamID, 0.0f, ThrowDistance, this);
		else
			MovementAbility->UseAbility(3.0f, location, 0.0f, ETeamIdentifier::None, 0.0f, ThrowDistance, this);

		ctfPlayerState->bIsThrowing = false;
		GetWorld()->GetTimerManager().SetTimer(AbilityOneTimerHandle, [this]()
			{
				CanUseAbilityOne = true;

			}, AbilityOneCoolDown, false);
	}

}
void ABaseCharacter::UseAbilityOne_Implementation()
{
	if (!this) {
		return;
	}
	
	if (CanUseAbilityOne && !bIsDead) 
	{

		ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Can Use Ability In %f"), ForwardVector.X));
		if (ctfPlayerState)	
		{
			ctfPlayerState->bIsThrowing = true;

			ctfPlayerState->bIsThrowing = true;
	
			if (ctfPlayerState->bIsThrowing)
			{
				GetWorld()->GetTimerManager().SetTimer(ThrowingTimer, this, &ABaseCharacter::SetThrowAbilityOne,0.5f, false);
				CanUseAbilityOne = false;
			}
		}
	}
}

void ABaseCharacter::SetThrowAbilityTwo_Implementation()
{
	if (!this) {
		return;
	}
	if (!bIsDead) {
		if (SecondAbility) {
			ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	//		FVector tmpLoc = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100);
//			location = FTransform(tmpLoc + GetActorRightVector() * 40.0f);
			location = FTransform(ThirdPersonCamera->GetRelativeLocation() + CameraBoom->GetRelativeLocation() + RootComponent->GetRelativeLocation() + ThirdPersonCamera->GetForwardVector() * 40.0f);

			FVector ThrowDistance = ThirdPersonCamera->GetForwardVector() * SmokeThrowLength + ThirdPersonCamera->GetUpVector() * SmokeThrowHeight;
			if (ACTFPlayerState * StateOfPlayer = GetPlayerState<ACTFPlayerState>())
				SecondAbility->UseAbility(3.0f, location, 0.0f, StateOfPlayer->teamID, 0.0f, ThrowDistance, this);
			else
				SecondAbility->UseAbility(3.0f, location, 0.0f, ETeamIdentifier::None, 0.0f, ThirdPersonCamera->GetForwardVector() * 1000.0f, this);

			ctfPlayerState->bIsThrowing = false;
			GetWorld()->GetTimerManager().SetTimer(AbilityTwoTimerHandle, [this]()
				{

					CanUseAbilityTwo = true;
				}, AbilityTwoCoolDown, false);
		}
	}
}


void ABaseCharacter::UseAbilityTwo_Implementation()
{
	if (!this) {
		return;
	}
	if (CanUseAbilityTwo && !bIsDead) {
		ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
		//TODO
		//Fill in when the ability class is finished.
		ctfPlayerState->bIsThrowing = true;

		if (ctfPlayerState->bIsThrowing)
		{
			GetWorld()->GetTimerManager().SetTimer(ThrowingTimer, this, &ABaseCharacter::SetThrowAbilityTwo,1.0f, false);
			CanUseAbilityTwo = false;
		}
	}
}

float ABaseCharacter::GetAbilityOneCooldown()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(AbilityOneTimerHandle);
}

float ABaseCharacter::GetAbilityTwoCooldown()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(AbilityTwoTimerHandle);
}

void ABaseCharacter::DropFlag()	{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->PlayerDropFlag();
	}
}

void ABaseCharacter::Slow()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Slowed"));
	bIsSlowed = true;
}

void ABaseCharacter::UnSlow()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UnSlowed"));
	bIsSlowed = false;
}

void ABaseCharacter::Death_Implementation()
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();

	//Below code is added by Declan from GameMode Team
	if(ctfPlayerState)
	{
		ctfPlayerState->OnDeath(this, RespawnTime);
	}
}

void ABaseCharacter::TakeDamage(float damage_)
{
	//Reduce CurrentHealth by damage
	CurrentHealth -= damage_;
	//If b isn't dead
	if (!bIsDead) {
		//Check if health is under 0
		if (CurrentHealth <= 0) {
			//Call Death Function and set dead to true
			bIsDead = true;
			Death();
		}
	}
}

void ABaseCharacter::SetIsSprinting_Implementation(bool bIsSprinting)
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->bIsSprinting = bIsSprinting;
	}
}

void ABaseCharacter::Respawn_Implementation()
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->OnRespawn();
	}
}


void ABaseCharacter::SetIsJumping_Implementation(bool bIsJumping)
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->bIsJumping = bIsJumping;
	}
}

void ABaseCharacter::SetIsSwinging_Implementation(bool bIsSwinging_)
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->bIsSwinging = bIsSwinging_;
	}
}

void ABaseCharacter::SetIsThrowing_Implementation(bool bIsThrowing)
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->bIsThrowing = bIsThrowing;
	}
}

void ABaseCharacter::SetIsDrawingBow_Implementation(bool bIsDrawingBow)
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->bIsDrawingBow = bIsDrawingBow;
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//TODO
	//Fill this in with character input *IF* we plan on doing input inside this class instead of a seperate one.

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprinting);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	//Binding ability and attacks
	PlayerInputComponent->BindAction("AbilityOne", IE_Pressed, this, &ABaseCharacter::UseAbilityOne);
	PlayerInputComponent->BindAction("AbilityTwo", IE_Pressed, this, &ABaseCharacter::UseAbilityTwo);
	
		
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Made by GameMode team
	PlayerInputComponent->BindAction("KillBind", IE_Pressed, this, &ABaseCharacter::Death);
	PlayerInputComponent->BindAction("DropFlag", IE_Pressed, this, &ABaseCharacter::DropFlag);
}