// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterShopItem.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void UCharacterShopItem::Initialize(
	UWorld* World,
	FCharacterShopData CharacterShopData)
{
	this->World = World;
	this->CharacterShopData = CharacterShopData;

	static FString ContextString = "Init character shop item";
	for (FName RowName : CharacterShopData.HealthUpgrade->GetRowNames())
	{
		FItemUpgradeData* HealthUpgrade = CharacterShopData.HealthUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		HealthUpgrades.Add(*HealthUpgrade);
	}

	for (FName RowName : CharacterShopData.HealthRegenUpgrade->GetRowNames())
	{
		FItemUpgradeData* HealthRegenUpgrade = CharacterShopData.HealthRegenUpgrade->FindRow<FItemUpgradeData>(RowName, ContextString, true);
		HealthRegenUpgrades.Add(*HealthRegenUpgrade);
	}
	
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.MaxHealth = HealthUpgrades[0].UpgradeValue;
	Player->CharacterConfig.HealthRegen = HealthRegenUpgrades[0].UpgradeValue;
}

UWorld* UCharacterShopItem::GetWorld() const
{
	return World;
}

void UCharacterShopItem::UpgradeHealth()
{
	CurrentHealthLevel++;
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= HealthUpgrades[CurrentHealthLevel].Price;
	Player->CharacterConfig.MaxHealth = HealthUpgrades[CurrentHealthLevel].UpgradeValue;
}

void UCharacterShopItem::UpgradeHealthRegen()
{
	CurrentHealthRegenLevel++;
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterConfig.CurrentMoney -= HealthRegenUpgrades[CurrentHealthRegenLevel].Price;
	Player->CharacterConfig.HealthRegen = HealthRegenUpgrades[CurrentHealthRegenLevel].UpgradeValue;
}