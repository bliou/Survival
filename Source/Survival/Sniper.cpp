// Fill out your copyright notice in the Description page of Project Settings.

#include "Sniper.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ASniper::ASniper()
	: Super()
{
	WeaponType = EWeaponType::ESniper;
	WeaponProjectile = EWeaponProjectile::EBullet;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
}

void ASniper::BeginPlay()
{
	Super::BeginPlay();

	if (FloatCurve)
	{
		TimelineComponent->AddInterpFloat(FloatCurve, InterpFunction, FName("Alpha"));
		TimelineComponent->SetLooping(false);
		TimelineComponent->SetIgnoreTimeDilation(true);
	}

	ScopeMaterial = UMaterialInstanceDynamic::Create(wScopeMaterial, this);
}

void ASniper::StartAiming()
{
	if (MyPawn->State != ECharacterState::EFire
		&& MyPawn->State != ECharacterState::EIdle)
		return;

	TimelineComponent->Play();
	WeaponMesh->SetHiddenInGame(true);
}

void ASniper::EndAiming()
{
	TimelineComponent->Reverse();
	WeaponMesh->SetHiddenInGame(false);
}

void ASniper::TimelineFloatReturn(float FieldOfView, float ScopeOpacity)
{
	MyPawn->FPSCameraComponent->SetFieldOfView(FieldOfView);

	ScopeMaterial->SetScalarParameterValue(FName(TEXT("ScopeOpacity")), (90.f - FieldOfView) / 45.f);
}