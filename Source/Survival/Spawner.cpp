// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "SurvivalGameStateBase.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::InstantiateZombie(FPickupItemData PickupItemData)
{
	FVector Location = GetActorLocation();
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	AZombie* ZombieObj = Cast<AZombie>(ZombieSpawn->GetDefaultObject());
	
	ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
	static const FString ContextString = TEXT("Zombie context");
	
	FZombieDataTable* ZombieData = ZombieObj->ZombieDataTable->FindRow<FZombieDataTable>(
		FName(*FString::FromInt(GameState->CurrentWave)),
		ContextString,
		true);
	ZombieObj->ZombieConfig.Damages = ZombieData->Damages;
	ZombieObj->ZombieConfig.Health = ZombieData->Health;
	ZombieObj->ZombieConfig.MovementSpeed = ZombieData->MovementSpeed;
	ZombieObj->ZombieConfig.PickupItemData = PickupItemData;

	GetWorld()->SpawnActor<AZombie>(ZombieSpawn, Location, Rotation, SpawnInfo);
}

