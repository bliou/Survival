// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInfoInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UGameInfoInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void LaunchGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<class UUserWidget> wMainMenuHUD;


protected:
	UUserWidget* MainMenuHUD;
};
