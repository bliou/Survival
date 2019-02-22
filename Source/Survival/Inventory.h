// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Inventory.generated.h"

class AMyCharacter;

UCLASS()
class SURVIVAL_API AInventory: public AActor
{
	GENERATED_BODY()

public:
	AInventory();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY()
	TArray<class AWeapon*> Weapons;

	void EquipWeapon(int32 Slot);
	void EquipPreviousWeapon();

	void AddWeapon(TSubclassOf<AWeapon> Weapon_BP);

protected:
	AMyCharacter* MyCharacter;
	EWeaponType PreviousWeaponSlot;
};
