// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BarricadeBuyPriceUpgrade.generated.h"


USTRUCT(BlueprintType)
struct FBarricadeBuyPriceUpgrade : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 BuyPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Price;
};
