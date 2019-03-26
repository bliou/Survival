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
		FItemUpgradeData* AmmoPriceUpgrade = WeaponShopData.AmmoPriceUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		AmmoPriceUpgrades.Add(*AmmoPriceUpgrade);
	}
	for (FName RowName : WeaponShopData.DamagesUpgrade->GetRowNames())
	{
		FItemUpgradeData* DamagesUpgrade = WeaponShopData.DamagesUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		DamageUpgrades.Add(*DamagesUpgrade);
	}
	for (FName RowName : WeaponShopData.RangeUpgrade->GetRowNames())
	{
		FItemUpgradeData* RangeUpgrade = WeaponShopData.RangeUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		RangeUpgrades.Add(*RangeUpgrade);
	}
	for (FName RowName : WeaponShopData.AmmosUpgrade->GetRowNames())
	{
		FItemUpgradeData* AmmosUpgrade = WeaponShopData.AmmosUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		AmmosUpgrades.Add(*AmmosUpgrade);
	}
	for (FName RowName : WeaponShopData.ReloadTimeUpgrade->GetRowNames())
	{
		FItemUpgradeData* ReloadTimeUpgrade = WeaponShopData.ReloadTimeUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		ReloadTimeUpgrades.Add(*ReloadTimeUpgrade);
	}

	AWeapon* Weapon = Cast<AWeapon>(WeaponShopData.Weapon_BP->GetDefaultObject());
	Weapon->WeaponConfig.Damages = DamageUpgrades[0].UpgradeValue;
	Weapon->WeaponConfig.WeaponRange = RangeUpgrades[0].UpgradeValue;
	Weapon->WeaponConfig.MaxAmmoInStock = AmmosUpgrades[0].UpgradeValue;
	Weapon->WeaponConfig.ReloadTime = ReloadTimeUpgrades[0].UpgradeValue;
}

void UWeaponShopItem::UpgradeAmmoPrice()
{
	CurrentAmmoPriceLevel++;

	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= AmmoPriceUpgrades[CurrentAmmoPriceLevel].Price;
}

void UWeaponShopItem::UpgradeDammages()
{
	CurrentDamageLevel++;

	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= DamageUpgrades[CurrentDamageLevel].Price;

	Player->Inventory->Weapons[(int)WeaponType]->WeaponConfig.Damages = DamageUpgrades[CurrentDamageLevel].UpgradeValue;
}

void UWeaponShopItem::UpgradeRange()
{
	CurrentRangeLevel++;

	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= RangeUpgrades[CurrentRangeLevel].Price;

	Player->Inventory->Weapons[(int)WeaponType]->WeaponConfig.WeaponRange = RangeUpgrades[CurrentRangeLevel].UpgradeValue;
}

void UWeaponShopItem::UpgradeAmmos()
{
	CurrentAmmosLevel++;

	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= AmmosUpgrades[CurrentAmmosLevel].Price;

	Player->Inventory->Weapons[(int)WeaponType]->WeaponConfig.MaxAmmoInStock = AmmosUpgrades[CurrentAmmosLevel].UpgradeValue;
}

void UWeaponShopItem::UpgradeReloadTime()
{
	CurrentReloadLevel++;

	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= ReloadTimeUpgrades[CurrentReloadLevel].Price;

	Player->Inventory->Weapons[(int)WeaponType]->WeaponConfig.ReloadTime = ReloadTimeUpgrades[CurrentReloadLevel].UpgradeValue;
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
	Player->CharacterConfig.CurrentMoney -= AmmoPriceUpgrades[CurrentAmmoPriceLevel].UpgradeValue * Ammos;

	Player->Inventory->AddAmmosToWeapon(WeaponType, Ammos);
}

UWorld* UWeaponShopItem::GetWorld() const
{
	return World;
}