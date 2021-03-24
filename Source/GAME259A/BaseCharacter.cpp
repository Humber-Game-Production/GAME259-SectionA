#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "CTFPlayerState.h"

// Sets default values
ABaseCharacter::ABaseCharacter() : bIsDead(false), bIsSlowed(false), bIsStunned(false), bIsSprinting(false), bIsThrowing(false), SprintMultiplier(1.5f), MaxHealth(100.0f), MaxWalkSpeed(1200.0f),
									CurrentHealth(MaxHealth), CurrentMoveSpeed(MaxWalkSpeed), JumpVelocity(500.0f), RespawnTime(3.0f), SlowMultiplier(0.25f)
{
	//Set the character to not rotate when the mouse is moved, only the camera is rotated.
 	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Set collision capsule.
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//Set this so the character does not turn to look in the direction they are moving.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//Set how fast they turn to look in the direction they are moving.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	//Set how fast the character jumps.
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	//Setup camera arm. This controls how far away the camera is from the character.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	//Setup the camera and attach it to the camera arm.
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	TeleportAbility = CreateDefaultSubobject<UBaseAbilityClass>(TEXT("TeleportAbility"));
	SecondAbility = CreateDefaultSubobject<UBaseAbilityClass>(TEXT("SecondAbility"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

//Called when the player is supposed to move left (Axis = -1) or right (Axis = 1).
void ABaseCharacter::MoveRight(float Axis)
{
	//FVector right = GetActorRightVector();

	//if (Axis < 0) right *= -1;
	//if (Axis == 0) right *= 0;
	if (bIsSlowed) {
		Axis = Axis * SlowMultiplier;
	}
	if (!bIsSprinting)
	{
		Axis = Axis * 1/SprintMultiplier;
	}
	AddMovementInput(GetActorRightVector(), Axis);
	//Walk(right);

}


void ABaseCharacter::Walk(FVector Direction) {

	AddMovementInput(Direction, CurrentMoveSpeed);
}

void ABaseCharacter::Sprint() {
	//CurrentMoveSpeed = MaxWalkSpeed * SprintMultiplier;
	//GetCharacterMovement()->MaxWalkSpeed = CurrentMoveSpeed;
	bIsSprinting = true;
}

void ABaseCharacter::StopSprinting()
{
//	CurrentMoveSpeed = MaxWalkSpeed;
	//GetCharacterMovement()->MaxWalkSpeed = CurrentMoveSpeed;
	bIsSprinting = false;
}

//Called when the "Jump" input is pressed. 
void ABaseCharacter::StartJump()
{
	if (!GetCharacterMovement()->IsFalling() && !GetWorld()->GetTimerManager().IsTimerActive(JumpTimer))
	{
		bIsJumping = true;
		GetWorld()->GetTimerManager().SetTimer(JumpTimer, [this]()
			{
				Jump();
				bIsJumping = false;
			},
			0.5f, false);
	}
}

//Called when the player is supposed to move forward (Axis = 1) or backward (Axis = -1).
void ABaseCharacter::MoveForward(float Axis)
{
	//FVector forward = GetActorForwardVector();
	//if (Axis < 0) forward *= -1;
	//if (Axis == 0) forward *= 0;
	if (bIsSlowed) {
		Axis = Axis * SlowMultiplier;
	}
	if (!bIsSprinting)
	{
		Axis = Axis * 1 / SprintMultiplier;
	}
	AddMovementInput(GetActorForwardVector(), Axis);
	//Walk(forward);
}

void ABaseCharacter::SetThrowAbilityOne()
{
	location = FTransform(GetActorLocation() + GetActorForwardVector() * 100.0f);
	if(ACTFPlayerState* StateOfPlayer = GetPlayerState<ACTFPlayerState>())
		TeleportAbility->UseAbility(3.0f, location, 0.0f, StateOfPlayer->teamID, 0.0f, ThirdPersonCamera->GetForwardVector() * 6000.0f, this);
	else 
		TeleportAbility->UseAbility(3.0f, location, 0.0f, ETeamIdentifier::None, 0.0f, ThirdPersonCamera->GetForwardVector() * 6000.0f, this);
	
	bIsThrowing = false;
}

void ABaseCharacter::UseAbilityOne()
{

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Can Use Ability In %f"), ForwardVector.X));

	bIsThrowing = true;
	
	if (bIsThrowing)
	{
		GetWorld()->GetTimerManager().SetTimer(ThrowingTimer, this, &ABaseCharacter::SetThrowAbilityOne,

			1.0f, false);
	}
}

void ABaseCharacter::SetThrowAbilityTwo()
{
	location = FTransform(GetActorLocation() + GetActorForwardVector() * 100.0f);
	if (ACTFPlayerState * StateOfPlayer = GetPlayerState<ACTFPlayerState>())
		SecondAbility->UseAbility(3.0f, location, 0.0f, StateOfPlayer->teamID, 0.0f, ThirdPersonCamera->GetForwardVector() * 1000.0f, this);
	else
		SecondAbility->UseAbility(3.0f, location, 0.0f, ETeamIdentifier::None, 0.0f, ThirdPersonCamera->GetForwardVector() * 1000.0f, this);

	bIsThrowing = false;
}


void ABaseCharacter::UseAbilityTwo()
{
	//TODO
	//Fill in when the ability class is finished.
	bIsThrowing = true;

	if (bIsThrowing)
	{
		GetWorld()->GetTimerManager().SetTimer(ThrowingTimer, this, &ABaseCharacter::SetThrowAbilityTwo,

			1.0f, false);
	}
}

void ABaseCharacter::DropFlag()
{
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->PlayerDropFlag();
	}
}

void ABaseCharacter::UseMeleeAttack()
{
	//TODO (Combat)
	bIsSwinging = true;
	
}

void ABaseCharacter::UseRangedAttack()
{
	//TODO (Combat)
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

void ABaseCharacter::Death()
{
	//Rag doll if the player is dead.
	GetMesh()->SetAllBodiesSimulatePhysics(true);

	FTimerHandle UnusedTimerHandle;

	GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &ABaseCharacter::Respawn, RespawnTime, false);

	//Below code is added by Declan from GameMode Team
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->OnDeath();
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

void ABaseCharacter::Respawn()
{
	ACTFPlayerState* ctfPlayerState = GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		if(ACTFGameState* gameState = Cast<ACTFGameState>(GetWorld()->GetGameState()))
		{
			gameState->listOfTeams[ctfPlayerState->teamID]->SpawnPlayer(this);
		}
	}
	this->Destroy();
}



// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSwinging)
	{
		bIsSwinging = false;
	}
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
	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ABaseCharacter::UseMeleeAttack);
	PlayerInputComponent->BindAction("RangedAttack", IE_Pressed, this, &ABaseCharacter::UseRangedAttack);
		
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	//Made by GameMode team
	PlayerInputComponent->BindAction("KillBind", IE_Pressed, this, &ABaseCharacter::Death);
	PlayerInputComponent->BindAction("DropFlag", IE_Pressed, this, &ABaseCharacter::DropFlag);
}