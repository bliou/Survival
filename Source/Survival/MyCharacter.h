// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"
#include "MyCharacter.generated.h"

UCLASS()
class SURVIVAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

	AWeapon *CurrentWeapon;
	int PreviousWeaponSlot;

	UPROPERTY()
	TArray<TSubclassOf<class AWeapon>> Weapons;

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Fire();

	void EquipGun();
	void EquipHeavyWeapon();
	void EquipPreviousWeapon();

	int CurrentWeaponSlot();
};
