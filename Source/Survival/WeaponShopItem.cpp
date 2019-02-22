// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void UWeaponShopItem::Initialize(UWorld* World, FWeaponShopData WeaponShopData)
{
	Super::Initialize(
		World,
		WeaponShopData.Name,
		WeaponShopData.Thumbnail,
		WeaponShopData.Price);

	CurrentWeaponLevel = 0;
	bISOwned = WeaponShopData.bISOwned;
	Weapon_BP = WeaponShopData.Weapon_BP;
}

void UWeaponShopItem::Buy()
{
	Super::Buy();

	bISOwned = true;
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->Inventory->AddWeapon(Weapon_BP);
}