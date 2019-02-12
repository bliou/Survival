// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"
#include "MyCharacter.generated.h"


UENUM(BlueprintType)
namespace ECharacterState
{
	enum CharacterState
	{
		EIdle			UMETA(DisplayName = "Idle"),
		EReload			UMETA(DisplayName = "Reload"),
		EEquip			UMETA(DisplayName = "Equip"),
		EFire			UMETA(DisplayName = "Fire")
	};
}


USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	int32 CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	int32 CurrentMoney;
};

UCLASS()
class SURVIVAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult &SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Reload();

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	AWeapon *CurrentWeapon;
	EWeaponType::WeaponType PreviousWeaponSlot;

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> ShotgunTest;

	UPROPERTY()
	TArray<class AWeapon*> Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	FCharacterData CharacterConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = State)
	TEnumAsByte<ECharacterState::CharacterState> State;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Timer)
	float ReloadTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Timer)
	float EquipTimer;

	void TakeDamages(float Damages);

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Fire();
	void StopAutoFiring();
	void StartReloading();

	void EquipDefaultWeapon();
	void EquipGun();
	void EquipHeavyWeapon();
	void EquipPreviousWeapon();
};
