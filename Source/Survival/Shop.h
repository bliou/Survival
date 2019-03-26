// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponShopItem.h"
#include "BarricadeShopItem.h"
#include "CharacterShopItem.h"
#include "Shop.generated.h"

UCLASS()
class SURVIVAL_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<UWeaponShopItem*> WeaponShopItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	TArray<UBarricadeShopItem*> BarricadeShopItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	UCharacterShopItem* CharacterShopItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Init)
	class UDataTable* WeaponsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Init)
	class UDataTable* BarricadesDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Init)
	FCharacterShopData CharacterShopData;
};
