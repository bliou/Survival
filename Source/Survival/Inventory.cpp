// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SurvivalGameStateBase.h"

void UInventory::Initialize(UWorld* World)
{
	// By default, the player has a gun
	Weapons.SetNum(5, false);
	PreviousWeaponSlot = EWeaponType::EGun;
	this->World = World;

//	Weapons[0] = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
	AShop* Shop = GameState->Shop;
	AddBarricades(
		EBarricadeType::EDumpster, 
		5,
		Shop->BarricadeShopItems[0]->BarricadeShopData.Barricade_BP);
}

UWorld* UInventory::GetWorld() const
{
	return World;
}

void UInventory::EquipWeapon(int32 Slot)
{
	if (!Weapons[Slot]
		|| MyCharacter->State == ECharacterState::EEquip
		|| MyCharacter->State == ECharacterState::EDead)
		return;
	if (MyCharacter->CurrentWeapon)
	{
		if (MyCharacter->CurrentWeapon->WeaponType == (EWeaponType)Slot)
			return;

		PreviousWeaponSlot = MyCharacter->CurrentWeapon->WeaponType;
		MyCharacter->CurrentWeapon->UnEquip();
	}

	MyCharacter->CurrentWeapon = Weapons[Slot];
	MyCharacter->CurrentWeapon->SetOwningPawn(MyCharacter);
	MyCharacter->CurrentWeapon->Equip();
}

void UInventory::EquipPreviousWeapon()
{
	EquipWeapon((int32)PreviousWeaponSlot);
}

void UInventory::AddWeapon(TSubclassOf<AWeapon> Weapon_BP)
{
	AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Weapon_BP);
	Weapons[(int)Weapon->WeaponType] = Weapon;
}

void UInventory::AddBarricades(
	EBarricadeType BarricadeType,
	int32 Quantity,
	TSubclassOf<ABarricade> Barricade_BP)
{
	if (!Barricades.Contains(BarricadeType))
	{
		FBarricadeInventory BarricadeInventory;
		BarricadeInventory.Quantity = Quantity;
		BarricadeInventory.Barricade_BP = Barricade_BP;

		Barricades.Add(BarricadeType, BarricadeInventory);
	}
	else
	{
		Barricades[BarricadeType].Quantity += Quantity;
	}
}

void UInventory::UpdateBarricadeMaxHealth(
	EBarricadeType BarricadeType,
	int32 NewMaxHealth)
{
	if (Barricades.Contains(BarricadeType))
	{
		ABarricade* Barricade = Cast<ABarricade>(Barricades[BarricadeType].Barricade_BP->GetDefaultObject());
		Barricade->BarricadeConfig.MaxHealth = NewMaxHealth;
	}
}

void UInventory::EquipBarricade(int32 Slot)
{
	EBarricadeType BarricadeType = (EBarricadeType)Slot;
	if (Barricades.Contains(BarricadeType)
		&& Barricades[BarricadeType].Quantity > 0)
	{
		MyCharacter->CurrentBarricade = GetWorld()->SpawnActor<ABarricade>(Barricades[BarricadeType].Barricade_BP);
		MyCharacter->CurrentBarricade->Equip(MyCharacter);
	}
}