// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Engine.h"
#include "MyCharacter.h"
#include "Barricade.h"
#include "ZombieController.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h"

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

	DamageComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageComponent"));
	DamageComponent->SetupAttachment(
		GetMesh(),
		TEXT("DamageSocket")
	);
	DamageComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnInflictDamages);

	DetectPlayerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectPlayerComponent"));
	DetectPlayerComponent->SetupAttachment(GetCapsuleComponent());
	DetectPlayerComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnDetectPlayerStart);
	DetectPlayerComponent->OnComponentEndOverlap.AddDynamic(this, &AZombie::OnDetectPlayerEnd);

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
	bCanInflictDamages = false;

	// Register the dropp item generator
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADroppedItemGenerator::StaticClass(), FoundActors);
	DroppedItemGenerator = Cast<ADroppedItemGenerator>(FoundActors[0]);
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AZombie::IsHeadShot(const FHitResult& Impact)
{
	return Impact.BoneName == "Head";
}

void AZombie::TakeDamages(const FHitResult& Impact, int Damages)
{
	if (State == EZombieState::EDying)
		return;

	if (IsHeadShot(Impact))
		Damages *= 5;

	ZombieConfig.Health -= Damages;
	if (ZombieConfig.Health <= 0)
	{
		State = EZombieState::EDying;
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->StopAllMontages(0.f);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle,
			this,
			&AZombie::KillZombie,
			ZombieConfig.DeathTimer,
			false);
	}
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

void AZombie::OnInflictDamages(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (!bCanInflictDamages)
		return;
	
	if (Cast<AZombieController>(GetController())->IsActorTargeted(OtherActor))
	{
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->TakeDamages(ZombieConfig.Damages);
			bCanInflictDamages = false;
		}
		ABarricade* Barricade = Cast<ABarricade>(OtherActor);
		if (Barricade)
		{
			Barricade->TakeDamages(ZombieConfig.Damages);
			bCanInflictDamages = false;
		}
	}
}

void AZombie::OnDetectPlayerStart(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (Cast<AMyCharacter>(OtherActor))
		bPlayerIsDetected = true;
}

void AZombie::OnDetectPlayerEnd(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (Cast<AMyCharacter>(OtherActor))
		bPlayerIsDetected = false;
}

void AZombie::AttackAnimationStart()
{
	if (State == EZombieState::EIdle)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(AttackMontage, 1.f);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, 
			this,
			&AZombie::AttackAnimationEnd,
			AttackMontage->GetPlayLength(), 
			false);

		State = EZombieState::EAttack;
	}
}

void AZombie::AttackAnimationEnd()
{
	State = EZombieState::EIdle;
}

void AZombie::KillZombie()
{
	DroppedItemGenerator->GenerateDroppedItem(
		ZombieConfig.ItemToDrop,
		GetActorLocation()
	);
	Destroy();
}