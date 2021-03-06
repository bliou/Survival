// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BarricadeShopData.h"
#include "ItemUpgradeData.h"
#include "BarricadeShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UBarricadeShopItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	FBarricadeShopData BarricadeShopData;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FItemUpgradeData> HealthUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentHealthLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<FItemUpgradeData> BuyPriceUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 CurrentBuyPriceLevel;

	UFUNCTION(BlueprintCallable)
	void Buy(int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void UpgradeHealth();

	UFUNCTION(BlueprintCallable)
	void UpgradeBuyPrice();

	void Initialize(UWorld* World, FBarricadeShopData BarricadeShopData);
	
	virtual class UWorld* GetWorld() const override;
	class UWorld* World;
};
