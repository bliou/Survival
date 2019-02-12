// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieController.h"
#include "Zombie.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

AZombieController::AZombieController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AZombieController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AZombie* Zombie = Cast<AZombie>(InPawn);
	MyPawn = Zombie;

	if (Zombie && Zombie->ZombieBehavior)
	{
		BlackboardComp->InitializeBlackboard(*Zombie->ZombieBehavior->BlackboardAsset);

		BehaviorComp->StartTree(*Zombie->ZombieBehavior);
	}
}

void AZombieController::SetPlayerFocus()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	MyFocus = Player;
	BlackboardComp->SetValueAsObject("PlayerReference", Player);
}

bool AZombieController::IsAlive()
{
	return MyPawn->State != EZombieState::EDying;
}

bool AZombieController::IsInAttackRange()
{
	if (MyFocus)
	{
		return MyPawn->ActorsInRange.Find(MyFocus) != INDEX_NONE;
	}
	return false;
}

bool AZombieController::ShouldMove()
{
	return !IsInAttackRange()
		&& MyPawn->State == EZombieState::EIdle;
}


void AZombieController::AttackAnimationStart()
{
	MyPawn->AttackAnimationStart();
}

void AZombieController::FaceTarget()
{
	FVector ZombieLocation = MyPawn->GetActorLocation();
	FRotator ZombieRotation = MyPawn->GetActorRotation();

	FVector TargetLocation = MyFocus->GetActorLocation();

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(ZombieLocation, TargetLocation);
	FRotator ZombieNewRotation = FMath::RInterpTo(ZombieRotation, LookAt, 0.1f, 2.0f);
	MyPawn->SetActorRotation(ZombieNewRotation);
}

bool AZombieController::IsActorTargeted(AActor* Actor)
{
	return Actor == MyFocus;
}