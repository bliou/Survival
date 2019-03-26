// Fill out your copyright notice in the Description page of Project Settings.

#include "Shotgun.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"

AShotgun::AShotgun()
{
	WeaponType = EWeaponType::EShotgun;
	WeaponProjectile = EWeaponProjectile::ESpread;
}

void AShotgun::Reload()
{
	WeaponConfig.CurrentAmmoInClip++;
	WeaponConfig.CurrentAmmoInStock--;
	if (WeaponConfig.CurrentAmmoInClip != WeaponConfig.MaxAmmoInClip
		&& WeaponConfig.CurrentAmmoInStock != 0)
	{
		UAnimInstance* AnimInstance = MyPawn->GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			FName CurrentSection = AnimInstance->Montage_GetCurrentSection();
			AnimInstance->Montage_JumpToSection(CurrentSection, ReloadMontage);
		}
	}
}