// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUpgradeWidget.h"

void UItemUpgradeWidget::CustomInit(
	const FText& Title,
	TArray<FItemUpgradeData> ItemUpgrades,
	int CurrentUpgrade)
{
	this->Title = Title;
	this->ItemUpgrades = ItemUpgrades;
	this->CurrentUpgrade = CurrentUpgrade;
}

void UItemUpgradeWidget::Upgrade_Implementation()
{
	CurrentUpgrade++;
}