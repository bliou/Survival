// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "DroppedItemGenerator.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState: uint8
{
	EIdle			UMETA(DisplayName = "Idle"),
	EAttack			UMETA(DisplayName = "Attack"),
	EDying			UMETA(DisplayName = "Dying")
};

USTRUCT(BlueprintType)
struct FZombieDataTable: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damages;
};

USTRUCT()
struct FZombieData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float DeathTimer;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damages;

	UPROPERTY(EditDefaultsOnly, Category = Item)
	FDroppedItemData DroppedItemData;
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

	UPROPERTY(EditDefaultsOnly, Category = Config)
	class UDataTable* ZombieDataTable;

	UPROPERTY(EditDefaultsOnly, Category = Collision)
	UBoxComponent* AttackRangeComponent;

	UPROPERTY(EditDefaultsOnly, Category = Collision)
	UCapsuleComponent* DamageComponent;

	UPROPERTY(EditDefaultsOnly, Category = Collision)
	UBoxComponent* DetectPlayerComponent;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree *ZombieBehavior;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Montage)
	class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EZombieState State;

	UFUNCTION()
	void TakeDamages(const FHitResult& Impact, int GunDamage);

	UFUNCTION()
	void OnStartAttack(
		class UPrimitiveComponent* HitComp, 
		class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnEndAttack(
		class UPrimitiveComponent* HitComp, 
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnInflictDamages(
		class UPrimitiveComponent* HitComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
		void OnDetectPlayerStart(
			class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult & SweepResult);

	UFUNCTION()
		void OnDetectPlayerEnd(
			class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	bool bPlayerIsDetected;

	void AttackAnimationStart();

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	bool bCanInflictDamages;

	TArray<AActor*> ActorsInRange;

protected:
	void KillZombie();
	void AttackAnimationEnd();

	ADroppedItemGenerator* DroppedItemGenerator;
};
