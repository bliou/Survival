// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Engine.h"
#include "ZombieController.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	AIControllerClass = AZombieController::StaticClass();
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();	

	bIsDying = false;
	GetCharacterMovement()->MaxWalkSpeed = ZombieConfig.MovementSpeed;
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDying)
	{
		ZombieConfig.DeathTimer -= DeltaTime;
		if (ZombieConfig.DeathTimer <= 0.f)
			Destroy();
	}
}

bool AZombie::IsHeadShot(const FHitResult& Impact)
{
	return Impact.BoneName == "Head";
}

void AZombie::Damaged(const FHitResult& Impact, int GunDamage)
{
	if (bIsDying)
		return;
	if (IsHeadShot(Impact))
		GunDamage *= 5;

	ZombieConfig.Health -= GunDamage;
	if (ZombieConfig.Health <= 0)
		bIsDying = true;
}

void AZombie::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, 1.f);
}