// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include <concrt.h>
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "..//BaseCharacter.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

// Sets default values
AGrapplingHook::AGrapplingHook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);
	SetRootComponent(Mesh);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetSphereRadius(10.0f);
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetNotifyRigidBodyCollision(true);
	SphereCollider->SetupAttachment(RootComponent);


	/*static ConstructorHelpers::FObjectFinder< USoundCue>GrapplingHookSoundObject(TEXT("SoundCue'/Game/VFX_Folder/SFX/AbilitySounds/GrapplingHookAbilitySoundCue.GrapplingHookAbilitySoundCue'"));
	if (GrapplingHookSoundObject.Succeeded()) {
		GrapplingHookSound = GrapplingHookSoundObject.Object;
		GrapplingHookAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GrapplingHookSoundObject"));
		GrapplingHookAudioComponent->SetupAttachment(RootComponent);
	}*/
}
// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentHit.AddDynamic(this, &AGrapplingHook::OnCompHit);
	Mesh->AddImpulse(LaunchSpeed * GetActorForwardVector());
	/*if (GrapplingHookAudioComponent && GrapplingHookSound) {
		GrapplingHookAudioComponent->SetSound(GrapplingHookSound);
	}*/
}

void AGrapplingHook::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGrapplingHook, BaseCharacter);
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrapplingHook::SetSpawner(AActor* BaseCharacter_)
{
	BaseCharacter = BaseCharacter_;
}

//sets objects velocity

void AGrapplingHook::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine && OtherActor) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetNotifyRigidBodyCollision(false);
	ABaseCharacter* otherPlayer = Cast<ABaseCharacter>(OtherActor);
	if (otherPlayer) {
		FVector CharacterLocation = otherPlayer->GetActorLocation();
		FVector GrappleLocation = Mesh->GetComponentLocation();

		FVector GrappleHorizontalVelocity = FVector(GrappleLocation.X - CharacterLocation.X, GrappleLocation.Y - CharacterLocation.Y, GrappleLocation.Z + 200.0f - CharacterLocation.Z);
		GrappleHorizontalVelocity *= Speed;
		FVector GrappleVerticalVelocity = FVector(0.0f, 0.0f, (float)sqrt((GrappleLocation.Z - CharacterLocation.Z) * SpeedVertical / 2) * SpeedVertical + pow(GrappleHorizontalVelocity.Size() / Speed / Speed, 2) * Gravity);
		//FVector GrappleVerticalVelocity = FVector(0.0f, 0.0f, (GrappleLocation.Z - CharacterLocation.Z) * SpeedVertical * );
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GrappleVVel: %f"), GrappleVerticalVelocity.Z));
		if (FGenericPlatformMath::IsNaN(GrappleVerticalVelocity.Z)) {
			GrappleVerticalVelocity.Z = 200;
		}

		otherPlayer->LaunchCharacter(GrappleHorizontalVelocity, true, true);
	}
	else {
		FVector CharacterLocation = BaseCharacter->GetActorLocation();
		FVector GrappleLocation = Mesh->GetComponentLocation();

		FVector GrappleHorizontalVelocity = FVector(GrappleLocation.X - CharacterLocation.X, GrappleLocation.Y - CharacterLocation.Y, GrappleLocation.Z + 200.0f - CharacterLocation.Z);
		GrappleHorizontalVelocity *= Speed;
		//FVector GrappleVerticalVelocity = FVector(0.0f, 0.0f, (float)sqrt((GrappleLocation.Z - CharacterLocation.Z) * SpeedVertical / 2) * SpeedVertical + pow(GrappleHorizontalVelocity.Size() / Speed / Speed, 2) * Gravity);
		//FVector GrappleVerticalVelocity = FVector(0.0f, 0.0f, (GrappleLocation.Z - CharacterLocation.Z) * SpeedVertical * );
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GrappleVVel: %f"), GrappleVerticalVelocity.Z));
	//	if (FGenericPlatformMath::IsNaN(GrappleVerticalVelocity.Z)) {
		//	GrappleVerticalVelocity.Z = 200;
		//}
		if (this) {
			if (BaseCharacter) {
				//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, CharacterLocation, CharacterRotation);
				if (Cast<ABaseCharacter>(BaseCharacter))
					Cast<ABaseCharacter>(BaseCharacter)->LaunchCharacter(GrappleHorizontalVelocity, true, true);
				//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, TpLocation, CharacterRotation);

					/*if (GrapplingHookAudioComponent && GrapplingHookSound) {
						GrapplingHookAudioComponent->Play(0.0f);
					}*/
			}


		}
	}
	otherPlayer = nullptr;
	SphereCollider->OnComponentHit.RemoveDynamic(this, &AGrapplingHook::OnCompHit);
	GetWorld()->GetTimerManager().SetTimer(GrappleTimer, this, &AGrapplingHook::OnDestroy, 3.0f, false);
}

void AGrapplingHook::OnDestroy()
{
	GetWorld()->GetTimerManager().ClearTimer(GrappleTimer);
	if (Mesh)
	{
		Mesh->UnregisterComponent();
		Mesh->DestroyComponent(true);
	}


	if (SphereCollider) {
		SphereCollider->UnregisterComponent();
		SphereCollider->DestroyComponent(true);
	}
	MarkPendingKill();
}