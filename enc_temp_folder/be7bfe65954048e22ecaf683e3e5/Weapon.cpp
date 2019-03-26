// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "MyCharacter.h"
#include "Zombie.h"
#include "Barricade.h"
#include "Engine.h"

AWeapon::AWeapon()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	bIsRecoiling = false;
	bUnEquip = false;

	WeaponConfig.CurrentWeaponSpread = 1.f;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DecreaseSpread(DeltaTime);
}

void AWeapon::Fire()
{
	if (bIsRecoiling
		|| WeaponConfig.CurrentAmmoInClip <= 0)
		return;

	// Stop all the animations
	UAnimInstance* AnimInstance = MyPawn->GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		FireSound,
		GetActorLocation()
	);

	switch (WeaponProjectile)
	{
	case EWeaponProjectile::EBullet:
	case EWeaponProjectile::EPierce:
		InstantFire();
		break;
	case EWeaponProjectile::ESpread:
		for (int32 i = 0; i < WeaponConfig.WeaponShots; ++i)
		{
			InstantFire();
		}
		break;
	}

	// After firing, remove the used bullets
	--WeaponConfig.CurrentAmmoInClip;
	bIsRecoiling = true;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		this,
		&AWeapon::EndRecoiling,
		WeaponConfig.RecoilTime,
		false);

	IncreaseSpread();
}

void AWeapon::InstantFire()
{
	UCameraComponent* FPSCamera = MyPawn->FPSCameraComponent;
	FVector FPSCameraWorldLocation = FPSCamera->GetComponentLocation();
	FVector FPSCameraForwardVector = FPSCamera->GetForwardVector();
	FVector StartTrace = WeaponMesh->GetSocketLocation("MS");

	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float ConeHalfAngle = FMath::DegreesToRadians(WeaponConfig.WeaponShots * 0.5f * WeaponConfig.CurrentWeaponSpread);
	const FVector ShootDir = WeaponRandomStream.VRandCone(FPSCameraForwardVector, ConeHalfAngle, ConeHalfAngle);
	FVector EndTrace = ShootDir * WeaponConfig.WeaponRange + FPSCameraWorldLocation;

	if (WeaponProjectile == EWeaponProjectile::EBullet
		|| WeaponProjectile == EWeaponProjectile::ESpread)
	{
		const FHitResult Impact = BulletTrace(StartTrace, EndTrace);

		ProcessInstantHit(
			Impact,
			StartTrace,
			EndTrace,
			WeaponConfig.CurrentWeaponSpread
		);
	}
	else
	{
		TArray<FHitResult> Impacts = PierceTrace(StartTrace, EndTrace);

		for (FHitResult Impact : Impacts)
		{
			ProcessInstantHit(
				Impact,
				StartTrace,
				EndTrace,
				WeaponConfig.CurrentWeaponSpread
			);
		}
	}

	// Display the particles of the weapon
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		FireParticles,
		WeaponMesh->GetSocketLocation("MS"),
		WeaponMesh->GetSocketRotation("MS"),
		true
	);
}

FHitResult AWeapon::BulletTrace(
	const FVector& TraceFrom,
	const FVector& TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("BulletFireTag"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		Hit, 
		TraceFrom, 
		TraceTo, 
		ECollisionChannel::ECC_PhysicsBody, 
		TraceParams);

	DrawDebugLine(GetWorld(), TraceFrom, TraceTo, FColor::Black, true, 20.f);

	return Hit;
}

TArray<FHitResult> AWeapon::PierceTrace(
	const FVector& TraceFrom,
	const FVector& TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("PierceFireTag"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	TArray<FHitResult> Hits;

	GetWorld()->LineTraceMultiByChannel(
		Hits,
		TraceFrom,
		TraceTo,
		ECollisionChannel::ECC_PhysicsBody,
		TraceParams);

	DrawDebugLine(GetWorld(), TraceFrom, TraceTo, FColor::Black, true, 20.f);
	return Hits;
}

void AWeapon::ProcessInstantHit(
	const FHitResult& Impact,
	const FVector& Origin,
	const FVector& End,
	float ReticleSpread)
{
	if (Impact.GetActor())
	{
		AZombie* zombie = Cast<AZombie>(Impact.GetActor());
		ABarricade* barricade = Cast<ABarricade>(Impact.GetActor());
		if (zombie)
		{
			zombie->TakeDamages(Impact, WeaponConfig.Damages);

			UGameplayStatics::PlaySoundAtLocation(
				this,
				BulletHitZombieSound,
				Impact.Location
			);
		}
		else if (barricade)
		{
			barricade->HittenByBullet(Impact, FVector());
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
	UAnimInstance* AnimInstance = MyPawn->GetMesh()->GetAnimInstance();
	if (AnimInstance != NULL)
	{
		AnimInstance->Montage_Play(EquipMontage, 1.f);
		MyPawn->State = ECharacterState::EEquip;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle,
			MyPawn,
			&AMyCharacter::EndEquipping,
			EquipMontage->GetPlayLength(),
			false);

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetHiddenInGame(false);
		AttachToComponent(
			MyPawn->GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			TEXT("WeaponSocket")
		);
	}
}

void AWeapon::UnEquip()
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	WeaponMesh->SetHiddenInGame(true);
	EndAiming();
	if (ReticleWidget)
		ReticleWidget->RemoveFromParent();
}

void AWeapon::StartReloading()
{
	if (WeaponConfig.CurrentAmmoInClip == WeaponConfig.MaxAmmoInClip
		|| WeaponConfig.CurrentAmmoInStock == 0)
		return;

	UAnimInstance* AnimInstance = MyPawn->GetMesh()->GetAnimInstance();
	if (AnimInstance != NULL)
	{
		float ReloadSpeed = ReloadMontage->GetPlayLength() / WeaponConfig.ReloadTime;
		AnimInstance->Montage_Play(ReloadMontage, ReloadSpeed);

		MyPawn->State = ECharacterState::EReload;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle,
			MyPawn,
			&AMyCharacter::EndReloading,
			WeaponConfig.ReloadTime,
			false);
		EndAiming();
	}
}

void AWeapon::Reload()
{
	int32 ReloadAmmo = FMath::Min(WeaponConfig.CurrentAmmoInStock, WeaponConfig.MaxAmmoInClip - WeaponConfig.CurrentAmmoInClip);
	WeaponConfig.CurrentAmmoInClip += ReloadAmmo;
	WeaponConfig.CurrentAmmoInStock -= ReloadAmmo;
}

void AWeapon::IncreaseSpread()
{
	WeaponConfig.CurrentWeaponSpread += WeaponConfig.WeaponIncreaseSpread;
	if (WeaponConfig.CurrentWeaponSpread > WeaponConfig.WeaponMaxSpread)
		WeaponConfig.CurrentWeaponSpread = WeaponConfig.WeaponMaxSpread;
}

void AWeapon::DecreaseSpread(float DeltaTime)
{
	if (WeaponConfig.CurrentWeaponSpread <= 1.f) {
		return;
	}

	float Decrease = DeltaTime * WeaponConfig.WeaponDecreaseSpread;
	WeaponConfig.CurrentWeaponSpread -= Decrease;
	if (WeaponConfig.CurrentWeaponSpread <= 1.f)
		WeaponConfig.CurrentWeaponSpread = 1.f;
}

void AWeapon::EndRecoiling()
{
	bIsRecoiling = false;

	// Reload if needed
	if (WeaponConfig.CurrentAmmoInClip <= 0
		&& WeaponConfig.CurrentAmmoInStock > 0)
		StartReloading();
}

void AWeapon::StartAiming()
{
}

void AWeapon::EndAiming()
{

}