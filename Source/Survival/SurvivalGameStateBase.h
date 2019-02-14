// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpawnManager.h"
#include "SurvivalGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EGameState: uint8
{
	EStartWave		UMETA(DisplayName = "Start a wave"),
	EInWave			UMETA(DisplayName = "In a wave"),
	EInBetweenWaves	UMETA(DisplayName = "Between waves")
};


/**
 * 
 */
UCLASS()
class SURVIVAL_API ASurvivalGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	ASurvivalGameStateBase();

protected:
	virtual void BeginPlay() override;

	bool IsWaveEnded();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	EGameState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Spawner)
	ASpawnManager* SpawnManager;
};
