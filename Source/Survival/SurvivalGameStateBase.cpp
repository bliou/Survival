// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalGameStateBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Zombie.h"

ASurvivalGameStateBase::ASurvivalGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASurvivalGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentWave = 1;
	CurrentState = EGameState::EStartWave;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnManager::StaticClass(), FoundActors);
	SpawnManager = Cast<ASpawnManager>(FoundActors[0]);
}

void ASurvivalGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EGameState::EStartWave:
		SpawnManager->StartWave();
		CurrentState = EGameState::EInWave;
		break;
	case EGameState::EInWave:
		if (IsWaveEnded())
		{
			CurrentState = EGameState::EInBetweenWaves;
		}
		break;
	case EGameState::EInBetweenWaves:
		break;
	}
}

bool ASurvivalGameStateBase::IsWaveEnded()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombie::StaticClass(), FoundActors);

	return FoundActors.Num() == 0 && SpawnManager->IsWaveInactive;
}