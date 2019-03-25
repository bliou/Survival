// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Sniper.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API ASniper : public AWeapon
{
	GENERATED_BODY()
	
public:
	ASniper();

	virtual void BeginPlay() override;

	virtual void StartAiming() override;
	virtual void EndAiming() override;

	UPROPERTY(EditDefaultsOnly, Category = Curves)
	class UCurveFloat* FloatCurve;

	UPROPERTY()
	class UTimelineComponent* TimelineComponent;

	FOnTimelineFloat InterpFunction;

	UPROPERTY(EditAnywhere, Category = Materials)
	UMaterialInterface* wScopeMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Materials)
	UMaterialInstanceDynamic* ScopeMaterial;

	UFUNCTION()
	void TimelineFloatReturn(float FieldOfView, float ScopeOpacity);
};
