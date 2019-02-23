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
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	EBarricadeType BarricadeType;

	virtual void Buy() override;

	void Initialize(UWorld* World, FBarricadeShopData BarricadeShopData);
};
