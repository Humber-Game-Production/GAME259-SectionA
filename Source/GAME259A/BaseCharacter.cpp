#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "CTFPlayerState.h"


// Sets default values
ABaseCharacter::ABaseCharacter() : bIsDead(false), bIsSlowed(false), bIsStunned(false), bIsSprinting(false), SprintMultiplier(1.5f), MaxHealth(100.0f), MaxWalkSpeed(1200.0f),
									CurrentHealth(MaxHealth), CurrentMoveSpeed(MaxWalkSpeed), RespawnTime(3.0f)
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


//Called when the player is supposed to move forward (Axis = 1) or backward (Axis = -1).
void ABaseCharacter::MoveForward(float Axis)
{
	//FVector forward = GetActorForwardVector();
	//if (Axis < 0) forward *= -1;
	//if (Axis == 0) forward *= 0;
	if (!bIsSprinting)
	{
		Axis = Axis * 1 / SprintMultiplier;
	}
	AddMovementInput(GetActorForwardVector(), Axis);
	//Walk(forward);
}

void ABaseCharacter::UseAbilityOne()
{
	//TODO
	//Fill in when the ability class is finished.
}

void ABaseCharacter::UseAbilityTwo()
{
	//TODO
	//Fill in when the ability class is finished.
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
}

void ABaseCharacter::UseRangedAttack()
{
	//TODO (Combat)
}

void ABaseCharacter::Death()
{
	//Rag doll if the player is dead.
	GetMesh()->SetSimulatePhysics(true);

	FTimerHandle UnusedTimerHandle;
	GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &ABaseCharacter::Respawn, RespawnTime, false);

	//Below code is added by Declan from GameMode Team
	ACTFPlayerState* ctfPlayerState = this->GetPlayerState<ACTFPlayerState>();
	if(ctfPlayerState)
	{
		ctfPlayerState->OnDeath();
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
	
	
	//bIsDead = false by default
	if (!bIsDead) {
		//Start dying - COMMENT THIS OUT IF YOU DON'T WANT CHARACTER TO DIE RANDOMLY AFTER A FEW SECONDS

		//when he loses all HP
		if (CurrentHealth <= 0) {
			bIsDead = true;
			Death();
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//TODO
	//Fill this in with character input *IF* we plan on doing input inside this class instead of a seperate one.

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
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