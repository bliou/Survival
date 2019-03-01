// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Barricade.h"
#include "BarricadeType.h"
#include "Inventory.generated.h"

class AMyCharacter;

USTRUCT(BlueprintType)
struct FBarricadeInventory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<ABarricade> Barricade_BP;
};

UCLASS()
class SURVIVAL_API UInventory: public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UWorld* World);

	UPROPERTY(EditDefaultsOnly, Category = Config)
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<class AWeapon*> Weapons;

	void EquipWeapon(int32 Slot);
	void EquipPreviousWeapon();

	void SetPreviousWeapon();

	void AddWeapon(TSubclassOf<AWeapon> Weapon_BP);
	void AddAmmoToRandomWeapon(int32 Clips);

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TMap<EBarricadeType, FBarricadeInventory> Barricades;

	void AddBarricades(
		EBarricadeType BarricadeType,
		int32 Quantity,
		TSubclassOf<ABarricade> Barricade_BP);

	void EquipBarricade(int32 Slot);
	void UnEquipBarricade();

	void UpdateBarricadeMaxHealth(EBarricadeType BarricadeType, int32 NewMaxHealth);

protected:
	AMyCharacter* MyCharacter;
	EWeaponType PreviousWeaponSlot;

	UWorld* World;
	virtual UWorld* GetWorld() const override;
};
