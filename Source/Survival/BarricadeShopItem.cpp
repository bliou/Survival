// Fill out your copyright notice in the Description page of Project Settings.

#include "BarricadeShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UBarricadeShopItem::Initialize(
	UWorld* World,
	FBarricadeShopData BarricadeShopData)
{
	Super::Initialize(
		World,
		BarricadeShopData.Name,
		BarricadeShopData.Thumbnail,
		BarricadeShopData.Price);

	BarricadeType = BarricadeShopData.BarricadeType;
	Barricade_BP = BarricadeShopData.Barricade_BP;
}

void UBarricadeShopItem::Buy(int Amount)
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= Price * Amount;
	
	Player->Inventory->AddBarricades(BarricadeType, Amount, Barricade_BP);
}