// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.h"
#include "ShopItem.h"
#include "WeaponShopData.h"
#include "WeaponShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UWeaponShopItem : public UShopItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int CurrentWeaponLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	bool bISOwned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TSubclassOf<AWeapon> Weapon_BP;

	UFUNCTION(BlueprintCallable)
	void Buy();

	void Initialize(UWorld* World, FWeaponShopData WeaponShopData);
};
