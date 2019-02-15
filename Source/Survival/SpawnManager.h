// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.h"
#include "Engine/DataTable.h"
#include "PickupItem.h"
#include "SpawnManager.generated.h"


USTRUCT(BlueprintType)
struct FWaveDataTable: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	FString WaveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	int32 ZombiesToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	int32 LifeItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	int32 MoneyItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	int32 AmmoItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	int32 MoneyValue;
};


UCLASS()
class SURVIVAL_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Wave)
	class UDataTable* WaveDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Text)
	FString WaveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Widget)
	TSubclassOf<UUserWidget> wStartWaveMessage;
	class UUserWidget* StartWaveMessage;

	UFUNCTION(BlueprintCallable)
	void StartWave();

	bool IsWaveInactive;

protected:
	TArray<ASpawner*> Spawners;
	TArray<FPickupItemData> AvailablePickupItems;

	int32 ZombiesToSpawnInWave;
	float Delay;

	void EndStartWave();
	void SetAvailabePickupItemsInRange(
		FPickupItemData PickupItemData,
		int32 StartIndex,
		int32 EndIndex);
};
