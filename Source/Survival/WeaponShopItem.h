// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.h"
#include "WeaponShopData.h"
#include "WeaponAmmoPriceUpgrade.h"
#include "WeaponShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UWeaponShopItem : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	FWeaponShopData WeaponShopData;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FWeaponAmmoPriceUpgrade> AmmoPriceUpgrades;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentAmmoPriceLevel;

	UFUNCTION(BlueprintCallable)
	void UpgradeAmmoPrice();

	UFUNCTION(BlueprintCallable)
	void Buy();

	UFUNCTION(BlueprintCallable)
	void BuyAmmos(int32 Ammos);

	void Initialize(UWorld* World, FWeaponShopData WeaponShopData);

	virtual class UWorld* GetWorld() const override;
	class UWorld* World;
};
