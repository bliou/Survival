// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterShopData.h"
#include "ItemUpgradeData.h"
#include "CharacterShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UCharacterShopItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		FCharacterShopData CharacterShopData;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		TArray<FItemUpgradeData> HealthUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		int CurrentHealthLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		TArray<FItemUpgradeData> HealthRegenUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		int CurrentHealthRegenLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		TArray<FItemUpgradeData> MovementSpeedUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		int CurrentMovementSpeedLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		TArray<FItemUpgradeData> PickupRangeUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		int CurrentPickupRangeLevel;

	UFUNCTION(BlueprintCallable)
		void UpgradeHealth();

	UFUNCTION(BlueprintCallable)
	void UpgradeHealthRegen();

	UFUNCTION(BlueprintCallable)
		void UpgradeMovementSpeed();

	UFUNCTION(BlueprintCallable)
		void UpgradePickupRange();

	void Initialize(UWorld* World, FCharacterShopData CharacterShopData);

	virtual class UWorld* GetWorld() const override;
	class UWorld* World;
};
