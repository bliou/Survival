// Fill out your copyright notice in the Description page of Project Settings.

#include "DroppedItemGenerator.h"

// Sets default values
ADroppedItemGenerator::ADroppedItemGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADroppedItemGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroppedItemGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADroppedItemGenerator::GenerateDroppedItem(
	EDroppedItemType DroppedItemType, 
	FVector Location)
{
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	switch (DroppedItemType)
	{
	case EDroppedItemType::ELife:
		GetWorld()->SpawnActor<ADroppedItem>(LifeItem, Location, Rotation, SpawnInfo);
		break;
	case EDroppedItemType::EMoney:
		GetWorld()->SpawnActor<ADroppedItem>(MoneyItem, Location, Rotation, SpawnInfo);
		break;
	}
}