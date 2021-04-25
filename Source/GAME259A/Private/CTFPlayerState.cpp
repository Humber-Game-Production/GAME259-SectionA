// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerState.h"
#include "GAME259A/GameMode/Team.h"
#include "GAME259A/GameMode/CTFGameState.h"
#include "GAME259A/GameMode/Flag.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACTFPlayerState::ACTFPlayerState(): teamID(ETeamIdentifier::Human), pointsEarned(0), kills(0), deaths(0),
                                    flagsCaptured(0), FlagHeld(nullptr), PlayerCanPickupFlag(true)
{
	bReplicates = true;
	bNetLoadOnClient = true;
}

void ACTFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME( ACTFPlayerState, teamID);
	DOREPLIFETIME( ACTFPlayerState, pointsEarned);
	DOREPLIFETIME( ACTFPlayerState, kills);
	DOREPLIFETIME( ACTFPlayerState, deaths);
	DOREPLIFETIME( ACTFPlayerState, flagsCaptured);
	DOREPLIFETIME( ACTFPlayerState, FlagHeld);
	DOREPLIFETIME( ACTFPlayerState, PlayerCanPickupFlag);
	DOREPLIFETIME( ACTFPlayerState, teamScoreDelegate);
	DOREPLIFETIME( ACTFPlayerState, respawnPlayerDelegate);
	DOREPLIFETIME( ACTFPlayerState, currentObjectiveDelegate);
	
	DOREPLIFETIME(ACTFPlayerState, AbilityOneCooldownRemaining);
	DOREPLIFETIME(ACTFPlayerState, AbilityTwoCooldownRemaining);

	
	DOREPLIFETIME( ACTFPlayerState, bIsSwinging);
	DOREPLIFETIME( ACTFPlayerState, bIsSprinting);
	DOREPLIFETIME( ACTFPlayerState, bIsJumping);
	DOREPLIFETIME( ACTFPlayerState, bIsThrowing);
	DOREPLIFETIME( ACTFPlayerState, bIsDrawingBow);
	DOREPLIFETIME( ACTFPlayerState, bIsGrappleHook);
}

void ACTFPlayerState::ResetStats()
{
	pointsEarned = 0;

	kills = 0;

	deaths = 0;

	flagsCaptured = 0;
}

void ACTFPlayerState::UpdateObjective(AFlag* newFlag)
{
	currentObjectiveDelegate.Broadcast(newFlag);
}

void ACTFPlayerState::PlayerDropFlag_Implementation()
{
	PlayerCanPickupFlag = true;
	if(FlagHeld)
	{
		GetWorld()->GetGameState<ACTFGameState>()->FlagDropped(FlagHeld);
		FlagHeld->Execute_Drop(FlagHeld);
		FlagHeld = nullptr;
		UpdateObjective(FlagHeld);
	}
}

void ACTFPlayerState::CaptureFlag_Implementation()
{
	if(FlagHeld)
	{
		flagsCaptured++;
		AddScore(FlagHeld->pointValue);
		FlagHeld->Capture();
		PlayerCanPickupFlag = true;
		FlagHeld = nullptr;
		UpdateObjective(FlagHeld);
	}
}

void ACTFPlayerState::OnDeath_Implementation(ACharacter* character, float respawnTime)
{
	deaths++;
	//Rag doll if the player is dead.
	character->GetMesh()->SetAllBodiesSimulatePhysics(true);
	character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FTimerHandle UnusedTimerHandle;

	GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &ACTFPlayerState::OnRespawn, respawnTime, false);
	
	PlayerDropFlag();
	PlayerCanPickupFlag = false;
}

void ACTFPlayerState::SetTeam(ETeamIdentifier team)
{
	teamID = team;
}

void ACTFPlayerState::SetFlagHeld(AFlag* FlagHeld_)	{
	FlagHeld = FlagHeld_;
	GetWorld()->GetGameState<ACTFGameState>()->FlagPickedUp(FlagHeld);
	UpdateObjective(FlagHeld);
}

void ACTFPlayerState::SetCanPickupFlag(bool PlayerCanPickupFlag_)	{
	PlayerCanPickupFlag = PlayerCanPickupFlag_;
}

bool ACTFPlayerState::GetCanPickupFlag() const	{
	return PlayerCanPickupFlag;
}

void ACTFPlayerState::AddScore(int32 amountOfPoints)
{
	pointsEarned += amountOfPoints;
	teamScoreDelegate.Broadcast(teamID, amountOfPoints);
	UE_LOG(LogTemp, Warning, TEXT("%d points added to team %d"), amountOfPoints, teamID);
}

void ACTFPlayerState::OnRespawn_Implementation()
{
	respawnPlayerDelegate.Broadcast(teamID, this);
	UE_LOG(LogTemp, Warning, TEXT("Respawn player delegate should have played"));

	APawn* originalPawn = GetPawn();
	
	PlayerDropFlag();
	
	if(ACTFGameState* gameState = GetWorld()->GetGameState<ACTFGameState>())
	{
		const ATeam* playersTeam = gameState->GetTeam(teamID);

		//if this team exists
		if(playersTeam)
		{
			//if this team has respawn points setup
			if(playersTeam->respawnPoints.Num() != 0)
			{
				//setup location, rotation and parameters for spawns
				const FVector location = playersTeam->respawnPoints[0]->GetActorLocation();
				const FRotator rotation = playersTeam->respawnPoints[0]->GetActorRotation();

				FActorSpawnParameters spawnP;
				spawnP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
				//Spawn the new playerActor and get its pawn
				AActor* playerActor = GetWorld()->SpawnActor(playersTeam->playerType, &location, &rotation, spawnP);
				if (playerActor) {
					APawn* newPawn = Cast<APawn>(playerActor);
					if (newPawn) {

						//Get the original playerController and detach it from its pawn
						AController* controller = originalPawn->GetController();
						controller->UnPossess();
						//attach the playerState and playerController to the new pawn
						newPawn->SetPlayerState(controller->GetPlayerState<ACTFPlayerState>());
						if (controller) {
							controller->Possess(newPawn);
							//controller->SetControlRotation(rotation);
							controller->ClientSetRotation(rotation);
						}
					}
				}
				//Destroy original pawn
				originalPawn->Destroy();

				//Exit function
                return;
			}
			UE_LOG(LogTemp, Warning, TEXT("Respawn points were not set up, using default playerStart to spawn players"));
		}
	}
	//use the default playerStart to spawn the player if there are no teams/no respawn points setup
	AActor* playerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	const FVector location = playerStart->GetActorLocation();
	const FRotator rotation = playerStart->GetActorRotation();
	FActorSpawnParameters spawnP;
	spawnP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//spawn the playerActor and get its pawn
	AActor* playerActor = GetWorld()->SpawnActor(GetWorld()->GetAuthGameMode()->DefaultPawnClass, &location, &rotation, spawnP);
	APawn* newPawn = Cast<APawn>(playerActor);

	//Get the original playerController and detach it from its pawn
	AController* controller = originalPawn->GetController();
	controller->UnPossess();
	//attach the playerState and playerController to the new pawn
	newPawn->SetPlayerState(controller->GetPlayerState<ACTFPlayerState>());
	controller->Possess(newPawn);
			
	//Destroy original pawn
	originalPawn->Destroy();
}
