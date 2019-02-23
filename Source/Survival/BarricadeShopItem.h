// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BarricadeShopData.h"
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

	UFUNCTION(BlueprintCallable)
	void Buy(int32 Amount);

	void Initialize(UWorld* World, FBarricadeShopData BarricadeShopData);


	virtual class UWorld* GetWorld() const override;
	class UWorld* World;
};
