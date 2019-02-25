// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "BarricadeType.h"
#include "Barricade.generated.h"

class AMyCharacter;

USTRUCT(BlueprintType)
struct FBarricadeData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	int32 CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	EBarricadeType BarricadeType;
};

UCLASS()
class SURVIVAL_API ABarricade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarricade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	class UStaticMeshComponent* BarricadeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Collision)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	FBarricadeData BarricadeConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Particles)
	class UParticleSystem* HittenParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Sounds)
	class USoundBase* BulletHitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Widget)
	TSubclassOf<UUserWidget> wEquippedWidget;
	class UUserWidget* EquippedWidget;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	// Equip the barricade to the player
	void Equip(AMyCharacter* MyCharacter);
	
	bool Place();
	void RotateBarricadeLeft();
	void RotateBarricadeRight();

	UFUNCTION()
		void OnStartColliding(
			class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndColliding(
			class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	void TakeDamages(float Damages);
	void HittenByBullet(
		const FHitResult& Impact,
		const FVector& ShootDir);

	bool bIsRotatingLeft;
	bool bIsRotatingRight;

protected:
	bool bIsPlaced;
	int NumActorsCollided;
};
