// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BarricadeHealthUpgrade.generated.h"


USTRUCT(BlueprintType)
struct FBarricadeHealthUpgrade : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Price;
};
