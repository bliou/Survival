// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void UWeaponShopItem::Initialize(UWorld* World, FWeaponShopData WeaponShopData)
{
	this->World = World;
	this->WeaponShopData = WeaponShopData;

	WeaponType = Cast<AWeapon>(WeaponShopData.Weapon_BP->GetDefaultObject())->WeaponType;

	static FString ContextString = "Init weapons shop items";
	for (FName RowName : WeaponShopData.AmmoPriceUpgrade->GetRowNames())
	{
		FWeaponAmmoPriceUpgrade* AmmoPriceUpdate = WeaponShopData.AmmoPriceUpgrade->FindRow<FWeaponAmmoPriceUpgrade>(RowName, ContextString, true);
		AmmoPriceUpgrades.Add(*AmmoPriceUpdate);
	}
}

void UWeaponShopItem::UpgradeAmmoPrice()
{
	CurrentAmmoPriceLevel++;
}

void UWeaponShopItem::Buy()
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= WeaponShopData.Price;
	
	Player->Inventory->AddWeapon(WeaponShopData.Weapon_BP);
	WeaponShopData.bISOwned = true;
}

void UWeaponShopItem::BuyAmmos(int32 Ammos)
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= AmmoPriceUpgrades[CurrentAmmoPriceLevel].AmmoPrice * Ammos;

	Player->Inventory->AddAmmosToWeapon(WeaponType, Ammos);
}

UWorld* UWeaponShopItem::GetWorld() const
{
	return World;
}