// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterShopData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterShopData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* HealthUpgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* HealthRegenUpgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* MovementSpeedUpgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	class UDataTable* PickupRangeUpgrade;
};