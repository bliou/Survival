// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItemsGenerator.h"
#include "SurvivalGameStateBase.h"

// Sets default values
APickupItemsGenerator::APickupItemsGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APickupItemsGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickupItemsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItemsGenerator::GeneratePickupItem(
	FPickupItemData PickupItemData,
	FVector Location)
{
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	APickupItem* PickupItem = NULL;
	switch (PickupItemData.PickupItemType)
	{
	case EPickupItemType::ELife:
		PickupItem = GetWorld()->SpawnActor<APickupItem>(LifeItem, Location, Rotation, SpawnInfo);
		break;
	case EPickupItemType::EMoney:
		PickupItem = GetWorld()->SpawnActor<APickupItem>(MoneyItem, Location, Rotation, SpawnInfo);
		break;
	}
	if (PickupItem)
		PickupItem->PickupItemConfig = PickupItemData;
}