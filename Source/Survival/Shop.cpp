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
	
	static const FString ContextString = TEXT("Weapons shop data context");
	for (FName RowName : WeaponShopsDataTable->GetRowNames())
	{
		FWeaponShopData* WeaponShopData = WeaponShopsDataTable->FindRow<FWeaponShopData>(RowName, ContextString, true);
		UWeaponShopItem* WeaponShopItem = NewObject<UWeaponShopItem>();
		WeaponShopItem->Initialize(GetWorld(), *WeaponShopData);
		WeaponShopItems.Add(WeaponShopItem);
	}
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

