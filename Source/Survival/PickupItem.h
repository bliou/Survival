// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	ENone			UMETA(DisplayName = "None"),
	ELife			UMETA(DisplayName = "Life"),
	EMoney			UMETA(DisplayName = "Money"),
	EAmmo			UMETA(DisplayName = "Ammo")
};

USTRUCT(BlueprintType)
struct FPickupItemData
{
	GENERATED_USTRUCT_BODY()

	FPickupItemData()
	{

	}

	FPickupItemData(EPickupItemType Type, int32 Val)
	{
		PickupItemType = Type;
		Value = Val;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	EPickupItemType PickupItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	int32 Value;
};

UCLASS()
class SURVIVAL_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Collision)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	FPickupItemData PickupItemConfig;

	UFUNCTION()
		void OnCollide(
			class UPrimitiveComponent* HitComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult & SweepResult);


protected:
	float LifeSpan;
	float ToggleVisibilityCurrentTimer;
	float ToggleVisibilityTimer;
};
