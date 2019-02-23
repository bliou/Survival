// Fill out your copyright notice in the Description page of Project Settings.

#include "BarricadeShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UBarricadeShopItem::Initialize(
	UWorld* World,
	FBarricadeShopData BarricadeShopData)
{
	this->World = World;
	this->BarricadeShopData = BarricadeShopData;
}

void UBarricadeShopItem::Buy(int Amount)
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= BarricadeShopData.Price * Amount;
	
	Player->Inventory->AddBarricades(
		BarricadeShopData.BarricadeType, 
		Amount, 
		BarricadeShopData.Barricade_BP);
}

UWorld* UBarricadeShopItem::GetWorld() const
{
	return World;
}