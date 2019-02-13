// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	if (wMainMenuHUD) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		MainMenuHUD = CreateWidget<UUserWidget>(this, wMainMenuHUD);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (MainMenuHUD)
		{
			//let add it to the view port
			MainMenuHUD->AddToViewport();
		}

		//Show the Cursor.
		bShowMouseCursor = true;
	}

}
