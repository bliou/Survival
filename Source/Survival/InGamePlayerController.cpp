// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlayerController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wPlayerHUD) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		PlayerHUD = CreateWidget<UUserWidget>(this, wPlayerHUD);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (PlayerHUD)
		{
			//let add it to the view port
			PlayerHUD->AddToViewport();
		}

		//Show the Cursor.
		bShowMouseCursor = false;
	}

}