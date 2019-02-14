// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "ZombieController.generated.h"

class AZombie;

/**
 * 
 */
UCLASS()
class SURVIVAL_API AZombieController : public AAIController
{
	GENERATED_BODY()

public:
	AZombieController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

	virtual void Possess(class APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void SetTargetFocus();

	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	bool ShouldMove();

	UFUNCTION(BlueprintCallable)
	void AttackAnimationStart();

	UFUNCTION(BlueprintCallable)
	void FaceTarget();

	bool IsActorTargeted(AActor* Actor);

protected:
	UPROPERTY()
	AZombie* MyPawn;

	UPROPERTY()
	AActor* MyTarget;

	bool IsInAttackRange();
	bool CanZombieReachPlayer(AActor* Player);
	AActor* GetClosestBarricade();
};
