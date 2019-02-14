// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	Delay = 0.f;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), FoundActors);
	for (AActor* FoundActor : FoundActors)
	{
		Spawners.Add(Cast<ASpawner>(FoundActor));
	}
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Delay <= 0.f)
	{
		ASpawner* Spawner = Spawners[FMath::RandRange(0, Spawners.Num() - 1)];
		Spawner->InstantiateZombie();
		Delay = 3.f;
	}
	else
	{
		Delay -= DeltaTime;
	}
}

