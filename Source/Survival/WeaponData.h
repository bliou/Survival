// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Clip)
		int32 MaxAmmoInStock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Clip)
		int32 MaxAmmoInClip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Clip)
		int32 CurrentAmmoInStock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Clip)
		int32 CurrentAmmoInClip;

	UPROPERTY(EditDefaultsOnly, Category = Time)
		float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = Time)
		float RecoilTime;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float WeaponShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spread)
		float WeaponMaxSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spread)
		float WeaponIncreaseSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spread)
		float WeaponDecreaseSpread;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Spread)
		float CurrentWeaponSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
		FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
		bool bAutoFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Damage)
		float Damages;
};
