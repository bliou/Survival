// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Engine.h"
#include "ZombieController.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActorsInRange.SetNum(20, false);

	AttackRangeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackRangeComponent"));
	AttackRangeComponent->SetupAttachment(GetCapsuleComponent());
	AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnStartAttack);
	AttackRangeComponent->OnComponentEndOverlap.AddDynamic(this, &AZombie::OnEndAttack);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	AIControllerClass = AZombieController::StaticClass();
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();	

	State = EZombieState::EIdle;
	GetCharacterMovement()->MaxWalkSpeed = ZombieConfig.MovementSpeed;
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EZombieState::EDying)
	{
		ZombieConfig.DeathTimer -= DeltaTime;
		if (ZombieConfig.DeathTimer <= 0.f)
			Destroy();
	}

	if (State == EZombieState::EAttack)
	{
		AttackTimer -= DeltaTime;
		if (AttackTimer <= 0.f)
			State = EZombieState::EIdle;
	}
}

bool AZombie::IsHeadShot(const FHitResult& Impact)
{
	return Impact.BoneName == "Head";
}

void AZombie::Damaged(const FHitResult& Impact, int GunDamage)
{
	if (State == EZombieState::EDying)
		return;
	if (IsHeadShot(Impact))
		GunDamage *= 5;

	ZombieConfig.Health -= GunDamage;
	if (ZombieConfig.Health <= 0)
		State = EZombieState::EDying;
}

void AZombie::OnStartAttack(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	ActorsInRange.Add(OtherActor);
}

void AZombie::OnEndAttack(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ActorsInRange.Remove(OtherActor);
}

void AZombie::Attack()
{
	if (State == EZombieState::EIdle)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(AttackMontage, 1.f);
		AttackTimer = AttackMontage->GetPlayLength();

		State = EZombieState::EAttack;
	}
}