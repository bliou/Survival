// Fill out your copyright notice in the Description page of Project Settings.

// Followed this tutorial : https://www.youtube.com/watch?v=ubVpYz_x_Ig&list=PLnJzIPglAtowzF6HjyaToS33WI2m5Y6S3&index=8

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "WeaponData.h"
#include "Weapon.generated.h"

class AMyCharacter;

UENUM(BlueprintType)
enum class EWeaponProjectile: uint8
{
	EBullet			UMETA(DisplayName = "Bullet"),
	ESpread			UMETA(DisplayName = "Spread"),
	EPierce			UMETA(DisplayName = "Pierce")
};

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	EGun			UMETA(DisplayName = "Gun"),
	EShotgun		UMETA(DisplayName = "Shotgun"),
	ERifle			UMETA(DisplayName = "Rifle"),
	ESniper			UMETA(DisplayName = "Sniper")
};

UCLASS()
class SURVIVAL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Config)
	EWeaponProjectile WeaponProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Collision)
	class UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	class UStaticMeshComponent* WeaponMesh;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Sounds)
	class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Sounds)
	class USoundBase* BulletHitWallSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Sounds)
	class USoundBase* BulletHitZombieSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Gameplay)
	class UMaterialInterface* BulletHole;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Particles)
	class UParticleSystem* FireParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Montage)
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Montage)
	class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widget)
	TSubclassOf<UUserWidget> wReticleWidget;
	class UUserWidget* ReticleWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Recoil)
	bool bIsRecoiling;

	bool bUnEquip;

protected:

	UFUNCTION()
	void InstantFire();

	FHitResult BulletTrace(const FVector& TraceFrom, const FVector& TraceTo) const;
	TArray<FHitResult> PierceTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	void ProcessInstantHit(
		const FHitResult& Impact,
		const FVector& Origin,
		const FVector& End,
		float ReticleSpread);

	AMyCharacter* MyPawn;

public:

	void SetOwningPawn(AMyCharacter* actor);
	void Equip();
	void UnEquip();
	void StartReloading();
	virtual void Reload();
	virtual void StartAiming();
	virtual void EndAiming();

	void EndRecoiling();

	void IncreaseSpread();
	void DecreaseSpread(float DeltaTime);
};
