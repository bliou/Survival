// Fill out your copyright notice in the Description page of Project Settings.

// Followed this tutorial : https://www.youtube.com/watch?v=ubVpYz_x_Ig&list=PLnJzIPglAtowzF6HjyaToS33WI2m5Y6S3&index=8

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include "Weapon.generated.h"


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


USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 ShotCosts;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponSpread;
};

UCLASS()
class SURVIVAL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

	UFUNCTION()
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::WeaponProjectile> WeaponProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision)
	class UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	class UStaticMeshComponent* WeaponMesh;

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
};
