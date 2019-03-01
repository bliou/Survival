// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.h"
#include "PickupItemsGenerator.generated.h"

UCLASS()
class SURVIVAL_API APickupItemsGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItemsGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GeneratePickupItem(
		FPickupItemData PickupItemData,
		FVector Location);

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Item)
	TSubclassOf<APickupItem> LifeItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Item)
	TSubclassOf<APickupItem> MoneyItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Item)
	TSubclassOf<APickupItem> AmmoItem;
};
