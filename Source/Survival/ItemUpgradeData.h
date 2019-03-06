// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemUpgradeData.generated.h"


USTRUCT(BlueprintType)
struct FItemUpgradeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	float UpgradeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Price;
};
