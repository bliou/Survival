// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zombie.h"
#include "Spawner.generated.h"

UCLASS()
class SURVIVAL_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InstantiateZombie(FPickupItemData PickupItemData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spawn)
	TSubclassOf<class AZombie> ZombieSpawn;
};
