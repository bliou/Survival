// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInfoInstance.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UGameInfoInstance::ShowMainMenu()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);;
	// If the main menu was not yet created, the create it
	if (!MainMenuHUD)
	{
		// Create the widget and store it.
		MainMenuHUD = CreateWidget<UUserWidget>(PlayerController, wMainMenuHUD);
	}
	MainMenuHUD->AddToViewport();
	PlayerController->bShowMouseCursor = true;
}

void UGameInfoInstance::LaunchGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Game");
}