// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.h"
#include "WeaponShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UWeaponShopItem : public UObject
{
	GENERATED_BODY()
	
public:
	UWeaponShopItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int CurrentWeaponLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	bool bISOwned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TSubclassOf<AWeapon> Weapon_BP;
};
