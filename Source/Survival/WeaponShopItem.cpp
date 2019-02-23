// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void UWeaponShopItem::Initialize(UWorld* World, FWeaponShopData WeaponShopData)
{
	this->World = World;
	this->WeaponShopData = WeaponShopData;
	CurrentWeaponLevel = 0;
}

void UWeaponShopItem::Buy()
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= WeaponShopData.Price;
	
	Player->Inventory->AddWeapon(WeaponShopData.Weapon_BP);
	WeaponShopData.bISOwned = true;
}

UWorld* UWeaponShopItem::GetWorld() const
{
	return World;
}