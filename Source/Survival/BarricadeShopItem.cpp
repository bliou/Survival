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

	static FString ContextString = "Init barricade shop items";
	for (FName RowName : BarricadeShopData.HealthUpgrade->GetRowNames())
	{
		FBarricadeHealthUpgrade* HealthUpgrade = BarricadeShopData.HealthUpgrade->FindRow<FBarricadeHealthUpgrade>(RowName, ContextString, true);
		HealthUpgrades.Add(*HealthUpgrade);
	}

	for (FName RowName : BarricadeShopData.BuyPriceUpgrade->GetRowNames())
	{
		FBarricadeBuyPriceUpgrade* BuyPriceUpgrade = BarricadeShopData.BuyPriceUpgrade->FindRow<FBarricadeBuyPriceUpgrade>(RowName, ContextString, true);
		BuyPriceUpgrades.Add(*BuyPriceUpgrade);
	}
	this->BarricadeShopData.Price = BuyPriceUpgrades[0].BuyPrice;
}

void UBarricadeShopItem::Buy(int Quantity)
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= BarricadeShopData.Price * Quantity;
	
	Player->Inventory->AddBarricades(
		BarricadeShopData.BarricadeType, 
		Quantity, 
		BarricadeShopData.Barricade_BP);
}

UWorld* UBarricadeShopItem::GetWorld() const
{
	return World;
}

void UBarricadeShopItem::UpgradeHealth()
{
	CurrentHealthLevel++;
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= HealthUpgrades[CurrentHealthLevel].Price;

	// Update the max health where needed
	int32 NewMaxHealth = HealthUpgrades[CurrentHealthLevel].Health;
	ABarricade* Barricade = Cast<ABarricade>(BarricadeShopData.Barricade_BP->GetDefaultObject());
	Barricade->BarricadeConfig.MaxHealth = NewMaxHealth;

	Player->Inventory->UpdateBarricadeMaxHealth(
		BarricadeShopData.BarricadeType,
		NewMaxHealth
	);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABarricade::StaticClass(), FoundActors);
	for (AActor* FoundActor : FoundActors)
	{
		Cast<ABarricade>(FoundActor)->BarricadeConfig.MaxHealth = NewMaxHealth;
	}
}

void UBarricadeShopItem::UpgradeBuyPrice()
{
	CurrentBuyPriceLevel++;
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= BuyPriceUpgrades[CurrentBuyPriceLevel].Price;

	// Update the max health where needed
	int32 NewBuyPrice = BuyPriceUpgrades[CurrentBuyPriceLevel].BuyPrice;
	BarricadeShopData.Price = NewBuyPrice;
}