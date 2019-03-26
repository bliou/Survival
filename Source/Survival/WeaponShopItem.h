// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.h"
#include "WeaponShopData.h"
#include "ItemUpgradeData.h"
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
	TArray<FItemUpgradeData> AmmoPriceUpgrades;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentAmmoPriceLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FItemUpgradeData> DamageUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentDamageLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FItemUpgradeData> RangeUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentRangeLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FItemUpgradeData> AmmosUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentAmmosLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FItemUpgradeData> ReloadTimeUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentReloadLevel;

	UFUNCTION(BlueprintCallable)
	void UpgradeAmmoPrice();

	UFUNCTION(BlueprintCallable)
	void UpgradeDammages();

	UFUNCTION(BlueprintCallable)
	void UpgradeRange();

	UFUNCTION(BlueprintCallable)
	void UpgradeAmmos();

	UFUNCTION(BlueprintCallable)
	void UpgradeReloadTime();

	UFUNCTION(BlueprintCallable)
	void Buy();

	UFUNCTION(BlueprintCallable)
	void BuyAmmos(int32 Ammos);

	void Initialize(UWorld* World, FWeaponShopData WeaponShopData);

	virtual class UWorld* GetWorld() const override;
	class UWorld* World;
};
