// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Weapon.h"
#include "WeaponShopData.generated.h"


USTRUCT(BlueprintType)
struct FWeaponShopData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	bool bISOwned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TSubclassOf<AWeapon> Weapon_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* AmmoPriceUpgrade;
};
