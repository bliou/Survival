// Fill out your copyright notice in the Description page of Project Settings.

// Followed this tutorial : https://www.youtube.com/watch?v=ubVpYz_x_Ig&list=PLnJzIPglAtowzF6HjyaToS33WI2m5Y6S3&index=8

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Weapon.generated.h"

class AMyCharacter;

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum WeaponProjectile
	{
		EBullet			UMETA(DisplayName = "Bullet"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile")
	};
}

UENUM(BlueprintType)
namespace EWeaponType
{
	enum WeaponType
	{
		EGun			UMETA(DisplayName = "Gun"),
		EShotgun		UMETA(DisplayName = "Shotgun"),
		ERifle			UMETA(DisplayName = "Rifle"),
		ESniper			UMETA(DisplayName = "Sniper")
	};
}


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 MaxAmmoInStock;

	UPROPERTY(EditDefaultsOnly, Category = Clip)
	int32 MaxAmmoInClip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Clip)
	int32 CurrentAmmoInStock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Clip)
	int32 CurrentAmmoInClip;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float RecoilTime;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spread)
	float WeaponMaxSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spread)
	float WeaponIncreaseSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Spread)
	float WeaponDecreaseSpread;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Spread)
	float CurrentWeaponSpread;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	bool bAutoFire;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damages;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	TEnumAsByte<EWeaponType::WeaponType> WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	TEnumAsByte<EWeaponProjectile::WeaponProjectile> WeaponProjectile;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> wReticleWidget;
	class UUserWidget* ReticleWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Recoil)
	bool bIsRecoiling;

protected:

	UFUNCTION()
	void InstantFire();

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

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
	void Reload();

	void EndRecoiling();

	void IncreaseSpread();
	void DecreaseSpread(float DeltaTime);
};
