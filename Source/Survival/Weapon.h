// Fill out your copyright notice in the Description page of Project Settings.

// Followed this tutorial : https://www.youtube.com/watch?v=ubVpYz_x_Ig&list=PLnJzIPglAtowzF6HjyaToS33WI2m5Y6S3&index=8

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
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
		EHeavyWeapon	UMETA(DisplayName = "HeavyWeapon")
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
	float WeaponSpread;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Config)
	float ReloadTime;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	TEnumAsByte<EWeaponType::WeaponType> WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	TEnumAsByte<EWeaponProjectile::WeaponProjectile> WeaponProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision)
	class UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	class UStaticMeshComponent* WeaponMesh;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* BulletHitWallSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* BulletHitZombieSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	class UMaterialInterface* BulletHole;

protected:

	UFUNCTION()
	void InstantFire();

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	void ProcessInstantHit(
		const FHitResult& Impact,
		const FVector& Origin,
		const FVector& ShootDir,
		int32 RandomSeed,
		float ReticleSpread);

	AMyCharacter* MyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Recoil)
	bool bIsRecoiling;
	float RecoilTimer;

public:

	void SetOwningPawn(AMyCharacter* actor);
	void Equip();
	void UnEquip();
	void Reload();
};
