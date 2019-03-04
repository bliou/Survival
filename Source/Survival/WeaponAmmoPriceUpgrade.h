// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponAmmoPriceUpgrade.generated.h"


USTRUCT(BlueprintType)
struct FWeaponAmmoPriceUpgrade : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 AmmoPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Price;
};
