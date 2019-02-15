// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroppedItem.h"
#include "DroppedItemGenerator.generated.h"



UCLASS()
class SURVIVAL_API ADroppedItemGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedItemGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateDroppedItem(
		EDroppedItemType DroppedItemType,
		FVector Location);


	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Item)
	TSubclassOf<ADroppedItem> LifeItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Item)
	TSubclassOf<ADroppedItem> MoneyItem;
};
