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

	CurrentWave = 0;
	CurrentState = EGameState::EInBetweenWaves;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnManager::StaticClass(), FoundActors);
	SpawnManager = Cast<ASpawnManager>(FoundActors[0]);
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShop::StaticClass(), FoundActors);
	Shop = Cast<AShop>(FoundActors[0]);

	EndBeginPlay();
}

void ASurvivalGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EGameState::EStartWave:
		++CurrentWave;
		SpawnManager->StartWave();
		CurrentState = EGameState::EInWave;
		break;
	case EGameState::EInWave:
		if (IsWaveEnded())
		{
			CurrentState = EGameState::EInBetweenWaves;
			SpawnManager->EndWave();
			AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Player->EndWave();

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABarricade::StaticClass(), FoundActors);

			for (AActor* FoundActor : FoundActors)
			{
				ABarricade* Barricade = Cast<ABarricade>(FoundActor);
				Barricade->BarricadeConfig.CurrentHealth = Barricade->BarricadeConfig.MaxHealth;
			}
		}
		break;
	}
}

bool ASurvivalGameStateBase::IsWaveEnded()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombie::StaticClass(), FoundActors);

	return FoundActors.Num() == 0 && SpawnManager->IsWaveInactive;
}

void ASurvivalGameStateBase::EndBeginPlay_Implementation()
{

}