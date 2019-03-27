// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemUpgradeData.h"
#include "ItemUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UItemUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CustomInit(
		const FText& Title,
		TArray<FItemUpgradeData> ItemUpgrades,
		int CurrentUpgrade);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemUpgradeData> ItemUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentUpgrade;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Upgrade();
};
