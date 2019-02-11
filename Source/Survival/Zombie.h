// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

USTRUCT()
struct FZombieData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Config)
	int32 Health;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float DeathTimer;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float MovementSpeed;
};

UCLASS()
class SURVIVAL_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsHeadShot(const FHitResult& Impact);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FZombieData ZombieConfig;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree *ZombieBehavior;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Montage)
	class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDying;

	UFUNCTION()
	void Damaged(const FHitResult& Impact, int GunDamage);

	void Attack();
};
