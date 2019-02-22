// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShopItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVAL_API UShopItem : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	int32 Price;

	UFUNCTION(BlueprintCallable)
	virtual void Buy();

	virtual class UWorld* GetWorld() const override;
	class UWorld* World;

	void Initialize(
		UWorld* World,
		FString Name,
		UTexture2D* Thumbnail, 
		int32 Price);
};
