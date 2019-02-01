// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"

AWeapon::AWeapon()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = dynamic_cast<USceneComponent*>(CollisionComp);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

void AWeapon::Fire()
{
	switch (WeaponProjectile)
	{
	case EWeaponProjectile::EBullet:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString(TEXT("Fire Bullet")));
		InstantFire();
		break;
	case EWeaponProjectile::ESpread:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString(TEXT("Fire Spread")));
		for (int32 i = 0; i < WeaponConfig.WeaponSpread; ++i)
		{
			InstantFire();
		}
		break;
	case EWeaponProjectile::EProjectile:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString(TEXT("Fire Projectile")));
		break;
	}
}

void AWeapon::InstantFire()
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float WeaponSpread = WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponSpread * 0.5);
	const FVector AimDir = WeaponMesh->GetSocketRotation("MS").Vector();
	const FVector StartTrace = WeaponMesh->GetSocketLocation("MS");
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	ProcessInstantHit(
		Impact,
		StartTrace,
		ShootDir,
		RandomSeed,
		WeaponSpread
	);

	DrawDebugSphere(GetWorld(), StartTrace, 1.f, 32, FColor::Black, true, 1000.f);
}

FHitResult AWeapon::WeaponTrace(
	const FVector& TraceFrom,
	const FVector& TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponFireTag"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, ECollisionChannel::ECC_EngineTraceChannel1, TraceParams);

	return Hit;
}

void AWeapon::ProcessInstantHit(
	const FHitResult& Impact,
	const FVector& Origin,
	const FVector& ShootDir,
	int32 RandomSeed,
	float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

	DrawDebugLine(GetWorld(), Origin, EndPoint, FColor::Black, true, 10000, 10);
}