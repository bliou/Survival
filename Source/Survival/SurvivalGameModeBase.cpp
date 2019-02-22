// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalGameModeBase.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ASurvivalGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), wInGameWidget);
	AddWidgetToViewPort(false, false);
}

void ASurvivalGameModeBase::SwitchWidget(EWidgetType WidgetType)
{
	if (WidgetType == CurrentWidgetType)
		return;

	CurrentWidget->RemoveFromParent();
	CurrentWidgetType = WidgetType;

	switch (WidgetType)
	{
	case EWidgetType::EInGame:
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), wInGameWidget);
		AddWidgetToViewPort(false, false);
		break;
	case EWidgetType::EInventory:
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), wInventoryWidget);
		AddWidgetToViewPort(true, true);
		break;
	case EWidgetType::EShop:
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), wShopWidget);
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