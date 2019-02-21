// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	

	for (auto BP_WeaponShopItem : BP_WeaponShopItems)
	{
		UObject* c = BP_WeaponShopItem->GetDefaultObject();
		WeaponShopItems.Add(Cast<UWeaponShopItem>(c));
	}
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

