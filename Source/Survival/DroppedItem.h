// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroppedItem.generated.h"

UENUM(BlueprintType)
enum class EDroppedItemType : uint8
{
	ENone			UMETA(DisplayName = "None"),
	ELife			UMETA(DisplayName = "Life"),
	EMoney			UMETA(DisplayName = "Money")
};

USTRUCT(BlueprintType)
struct FDroppedItemData
{
	GENERATED_USTRUCT_BODY()

	FDroppedItemData()
	{

	}

	FDroppedItemData(EDroppedItemType Type, int32 Val)
	{
		DroppedItemType = Type;
		Value = Val;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	EDroppedItemType DroppedItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	int32 Value;
};


UCLASS()
class SURVIVAL_API ADroppedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedItem();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Collision)
	FDroppedItemData DroppedItemConfig;

	UFUNCTION()
	void OnCollide(
		class UPrimitiveComponent* HitComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);
};
