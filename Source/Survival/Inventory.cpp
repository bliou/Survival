// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AInventory::AInventory()
{
	// By default, the player has a gun
	Weapons.SetNum(5, false);
	PreviousWeaponSlot = EWeaponType::EGun;
}

void AInventory::BeginPlay()
{
	Weapons[0] = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AInventory::EquipWeapon(int32 Slot)
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

void AInventory::EquipPreviousWeapon()
{
	EquipWeapon((int32)PreviousWeaponSlot);
}
