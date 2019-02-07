// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "MyCharacter.h"
#include "Zombie.h"
#include "Engine.h"

AWeapon::AWeapon()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = dynamic_cast<USceneComponent*>(CollisionComp);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponConfig.CurrentAmmoInStock = WeaponConfig.MaxAmmoInStock;
	WeaponConfig.CurrentAmmoInClip = WeaponConfig.MaxAmmoInClip;
}

void AWeapon::Fire()
{
	if (WeaponConfig.CurrentAmmoInClip <= 0) {
		Reload();
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(
		this,
		FireSound,
		GetActorLocation()
	);

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

	// After firing, remove the used bullets
	--WeaponConfig.CurrentAmmoInClip;
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

	bool r = GetWorld()->LineTraceSingleByChannel(
		Hit, 
		TraceFrom, 
		TraceTo, 
		ECollisionChannel::ECC_PhysicsBody, 
		TraceParams);

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

	if (Impact.GetActor())
	{
		AZombie* zombie = Cast<AZombie>(Impact.GetActor());
		if (zombie)
		{
			zombie->Damaged(Impact, 100);

			UGameplayStatics::PlaySoundAtLocation(
				this,
				BulletHitZombieSound,
				Impact.Location
			);
		}
		else
		{
			ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Impact.Location, FRotator());
			decal->SetDecalMaterial(BulletHole);
			decal->SetLifeSpan(2.0f);
			decal->GetDecal()->DecalSize = FVector(10.f, 10.f, 10.f);

			UGameplayStatics::PlaySoundAtLocation(
				this,
				BulletHitWallSound,
				Impact.Location
			);
		}
	}
}

void AWeapon::SetOwningPawn(AMyCharacter* actor)
{
	Instigator = actor;
	MyPawn = actor;
}

void AWeapon::Equip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetHiddenInGame(false);
	AttachToComponent(
		MyPawn->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		TEXT("WeaponSocket")
	);
}

void AWeapon::UnEquip()
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	WeaponMesh->SetHiddenInGame(true);
}

void AWeapon::Reload()
{
	if (WeaponConfig.CurrentAmmoInClip == WeaponConfig.MaxAmmoInClip)
		return;

	int32 ReloadAmmo = FMath::Min(WeaponConfig.CurrentAmmoInStock, WeaponConfig.MaxAmmoInClip - WeaponConfig.CurrentAmmoInClip);
	WeaponConfig.CurrentAmmoInClip += ReloadAmmo;
	WeaponConfig.CurrentAmmoInStock -= ReloadAmmo;

	MyPawn->bIsReloading = true;
	MyPawn->ReloadTimer = WeaponConfig.ReloadTime;
}