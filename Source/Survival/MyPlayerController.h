// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<class UUserWidget> wMainMenu;

	// Variable to hold the widget After Creating it.
	UUserWidget* MyMainMenu;

	// Override BeginPlay()
	virtual void BeginPlay() override;
};
