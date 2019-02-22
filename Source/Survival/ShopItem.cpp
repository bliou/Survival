// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UShopItem::Initialize(
	UWorld* World,
	FString Name,
	UTexture2D* Thumbnail,
	int32 Price)
{
	this->World = World;
	this->Name = Name;
	this->Thumbnail = Thumbnail;
	this->Price = Price;
}

void UShopItem::Buy()
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= Price;
}

UWorld* UShopItem::GetWorld() const
{
	return World;
}