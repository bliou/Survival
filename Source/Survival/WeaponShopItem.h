// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.h"
#include "WeaponShopData.h"
#include "WeaponShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UWeaponShopItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int CurrentWeaponLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	FWeaponShopData WeaponShopData;

	UFUNCTION(BlueprintCallable)
	void Buy();

	void Initialize(UWorld* World, FWeaponShopData WeaponShopData);

	virtual class UWorld* GetWorld() const override;
	class UWorld* World;
};
