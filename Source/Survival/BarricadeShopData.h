// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Barricade.h"
#include "BarricadeType.h"
#include "BarricadeShopData.generated.h"


USTRUCT(BlueprintType)
struct FBarricadeShopData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UTexture2D* Thumbnail;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	int32 Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	EBarricadeType BarricadeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TSubclassOf<ABarricade> Barricade_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* HealthUpgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* BuyPriceUpgrade;
};
