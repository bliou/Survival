// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameModeBase.generated.h"

UENUM()
enum class EWidgetType : uint8
{
	EInGame,
	EInventory,
	EShop
};

/**
 * 
 */
UCLASS()
class SURVIVAL_API ASurvivalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Widget)
	TSubclassOf<UUserWidget> wInGameWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Widget)
	TSubclassOf<UUserWidget> wInventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Widget)
	TSubclassOf<UUserWidget> wShopWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Widget)
	class UUserWidget* CurrentWidget;

	UFUNCTION(BlueprintCallable)
	void SwitchWidget(EWidgetType WidgetType);

	EWidgetType CurrentWidgetType;

protected:
	void AddWidgetToViewPort(
		bool bShowMouseCursor,
		bool bEnableClickEvents);
};
