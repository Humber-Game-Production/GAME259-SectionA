#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABaseCharacter::ABaseCharacter() : bIsDead(false), bIsSlowed(false), bIsStunned(false), bIsSprinting(false), SprintMultiplier(1.5f), MaxHealth(100.0f), MaxWalkSpeed(1.0f),
									CurrentHealth(MaxHealth), CurrentMoveSpeed(MaxWalkSpeed)
{
	//Set the character to not rotate when the mouse is moved, only the camera is rotated.
 	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Set collision capsule.
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//Set this so the character turns to look in the direction they are moving.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Set how fast they turn to look in the direction they are moving.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	//Set how fast the character jumps.
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = 0.2f;

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
	FVector right = GetActorRightVector();

	if (Axis < 0) right *= -1;

	if (UKismetMathLibrary::Abs(Axis > 0.5)) Sprint(right);

	else Walk(right);

}


void ABaseCharacter::Walk(FVector Direction) {

	AddMovementInput(Direction, CurrentMoveSpeed);
}

void ABaseCharacter::Sprint(FVector Direction) {

AddMovementInput(Direction, CurrentMoveSpeed * SprintMultiplier);
}
//Called when the player is supposed to move forward (Axis = 1) or backward (Axis = -1).
void ABaseCharacter::MoveForward(float Axis)
{
	
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
}

void ABaseCharacter::Respawn()
{
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHealth <= 0)
	{
		bIsDead = true;
		Death();
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

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	//Binding ability and attacks
	PlayerInputComponent->BindAction("AbilityOne", IE_Pressed, this, &ABaseCharacter::UseAbilityOne);
	PlayerInputComponent->BindAction("AbilityTwo", IE_Pressed, this, &ABaseCharacter::UseAbilityTwo);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ABaseCharacter::UseMeleeAttack);
	PlayerInputComponent->BindAction("RangedAttack", IE_Pressed, this, &ABaseCharacter::UseRangedAttack);
}