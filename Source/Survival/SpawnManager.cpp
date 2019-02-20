// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "SurvivalGameStateBase.h"

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
	IsWaveInactive = true;
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsWaveInactive)
		return;

	if (Delay <= 0.f)
	{
		ASpawner* Spawner = Spawners[FMath::RandRange(0, Spawners.Num() - 1)];
		int AvailableItemIndex = FMath::RandRange(0, AvailablePickupItems.Num() - 1);
		Spawner->InstantiateZombie(AvailablePickupItems[AvailableItemIndex]);
		AvailablePickupItems.RemoveAt(AvailableItemIndex);
		ZombiesToSpawnInWave--;
		if (ZombiesToSpawnInWave <= 0)
		{
			IsWaveInactive = true;
		}
		Delay = FMath::RandRange(1.5f, 3.f);
	}
	else
	{
		Delay -= DeltaTime;
	}
}

void ASpawnManager::StartWave()
{
	if (!WaveMessage)
		WaveMessage = CreateWidget<UUserWidget>(GetWorld(), wStartWaveMessage);
	
	ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
	static const FString ContextString = TEXT("Wave context");
	
	FWaveDataTable* CurrentWaveConfig = WaveDataTable->FindRow<FWaveDataTable>(
		FName(*FString::FromInt(GameState->CurrentWave)),
		ContextString, 
		true);

	ZombiesToSpawnInWave = CurrentWaveConfig->ZombiesToSpawn;
	WaveName = CurrentWaveConfig->WaveName;
	IsWaveInactive = false;

	// Create the dropped items for the wave
	AvailablePickupItems.Empty();
	AvailablePickupItems.SetNum(ZombiesToSpawnInWave, false);
	int LifeItemsNum = CurrentWaveConfig->LifeItems;
	int MoneyItemsNum = CurrentWaveConfig->MoneyItems;
	SetAvailabePickupItemsInRange(
		FPickupItemData(EPickupItemType::ELife, 0),
		0, 
		LifeItemsNum);
	SetAvailabePickupItemsInRange(
		FPickupItemData(EPickupItemType::EMoney, CurrentWaveConfig->MoneyValue),
		LifeItemsNum,
		LifeItemsNum + MoneyItemsNum);
	SetAvailabePickupItemsInRange(
		FPickupItemData(EPickupItemType::ENone, 0),
		LifeItemsNum + MoneyItemsNum,
		ZombiesToSpawnInWave);

	WaveMessage->AddToViewport();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		this,
		&ASpawnManager::EndWaveMessage,
		1.5f,
		false);
}

void ASpawnManager::EndWave()
{
	WaveMessage->AddToViewport();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		this,
		&ASpawnManager::EndWaveMessage,
		1.5f,
		false);
}

void ASpawnManager::EndWaveMessage()
{
	WaveMessage->RemoveFromParent();
}

void ASpawnManager::SetAvailabePickupItemsInRange(
	FPickupItemData PickupItemData,
	int32 StartIndex,
	int32 EndIndex)
{
	for (int32 i = StartIndex; i < EndIndex; ++i)
	{
		AvailablePickupItems[i] = PickupItemData;
	}
}