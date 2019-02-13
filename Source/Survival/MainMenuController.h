// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<class UUserWidget> wMainMenuHUD;

	// Variable to hold the widget After Creating it.
	UUserWidget* MainMenuHUD;

	// Override BeginPlay()
	virtual void BeginPlay() override;
};
