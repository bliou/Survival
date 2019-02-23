// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopItem.h"
#include "BarricadeShopData.h"
#include "BarricadeShopItem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UBarricadeShopItem : public UShopItem
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	EBarricadeType BarricadeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TSubclassOf<ABarricade> Barricade_BP;

	UFUNCTION(BlueprintCallable)
	void Buy(int32 Amount);

	void Initialize(UWorld* World, FBarricadeShopData BarricadeShopData);
};
