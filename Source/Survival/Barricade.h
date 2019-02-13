// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Barricade.generated.h"


USTRUCT(BlueprintType)
struct FBarricadeData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	int32 CurrentHealth;
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

	void TakeDamages(float Damages);
};
