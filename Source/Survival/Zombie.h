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
};

UCLASS()
class SURVIVAL_API AZombie : public APawn
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


	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FZombieData ZombieConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDying;

	UFUNCTION()
	void Damaged(const FHitResult& Impact, int GunDamage);
};
