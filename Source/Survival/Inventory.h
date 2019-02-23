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
	int32 Amount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<ABarricade> Barricade_BP;
};

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

	UPROPERTY()
	TMap<EBarricadeType, FBarricadeInventory> Barricades;

	void AddBarricades(
		EBarricadeType BarricadeType,
		int32 Amount,
		TSubclassOf<ABarricade> Barricade_BP);

protected:
	AMyCharacter* MyCharacter;
	EWeaponType PreviousWeaponSlot;
};
