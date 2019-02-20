// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalGameModeBase.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ASurvivalGameModeBase::BeginPlay()
{
	InGameWidget = CreateWidget<UUserWidget>(GetWorld(), wInGameWidget);
	InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), wInventoryWidget);
	ShopWidget = CreateWidget<UUserWidget>(GetWorld(), wShopWidget);

	CurrentWidget = InGameWidget;
	CurrentWidgetType = EWidgetType::EInGame;
	AddWidgetToViewPort(false, false);
}

void ASurvivalGameModeBase::SwitchWidget(EWidgetType WidgetType)
{
	CurrentWidget->RemoveFromParent();
	CurrentWidgetType = WidgetType;

	switch (WidgetType)
	{
	case EWidgetType::EInGame:
		CurrentWidget = InGameWidget;
		AddWidgetToViewPort(false, false);
		break;
	case EWidgetType::EInventory:
		CurrentWidget = InventoryWidget;
		AddWidgetToViewPort(true, true);
		break;
	case EWidgetType::EShop:
		CurrentWidget = ShopWidget;
		AddWidgetToViewPort(true, true);
		break;
	}
}

void ASurvivalGameModeBase::AddWidgetToViewPort(
	bool bShowMouseCursor,
	bool bEnableClickEvents)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bEnableClickEvents = bEnableClickEvents;
	PlayerController->bShowMouseCursor = bShowMouseCursor;

	CurrentWidget->AddToViewport();
}