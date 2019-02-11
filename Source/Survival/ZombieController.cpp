// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieController.h"
#include "Zombie.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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

bool AZombieController::IsInAttackRange()
{
	if (MyFocus)
	{
		return MyPawn->ActorsInRange.Find(MyFocus) != INDEX_NONE;
	}
	return false;
}

bool AZombieController::IsAttacking()
{
	return MyPawn->State == EZombieState::EAttack;
}


void AZombieController::Attack()
{
	MyPawn->Attack();
}