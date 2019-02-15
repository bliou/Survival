// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroppedItem.generated.h"

UENUM(BlueprintType)
enum class EDroppedItemType : uint8
{
	ENone			UMETA(DisplayName = "No item to drop"),
	ELife			UMETA(DisplayName = "Drop life"),
	EMoney			UMETA(DisplayName = "Drop money")
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	EDroppedItemType DroppedItemType;

	UFUNCTION()
	void OnCollide(
		class UPrimitiveComponent* HitComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);
};
