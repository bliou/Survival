// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieController.h"
#include "Zombie.h"
#include "Barricade.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "Engine.h"
#include <limits>

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

void AZombieController::SetTargetFocus()
{
	AActor* Target = NULL;
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MyPawn->bPlayerIsDetected)
	{
		// If the player is in range for the zombie to focus on,
		// check if the path is valid
		if (CanZombieReachPlayer(Player))
		{
			// If it is, the player is the target
			Target = Player;
		}
		else
		{
			// Otherwise, focus on the closest barricade
			Target = GetClosestBarricade();
		}
	}
	else 
	{
		// If the player is not in focus range,
		// focus on the closest barricade
		Target = GetClosestBarricade();
		if (!Target)
		{
			// If there are no barricades left, then focus on
			// the player
			Target = Player;
		}
	}
	AActor* Actor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetReference"));
	if (Target != Actor)
	{
		BlackboardComp->SetValueAsObject("NextTargetReference", Target);
		MyTarget = Target;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, Target->GetClass()->GetFName().ToString());
	}
}

bool AZombieController::IsAlive()
{
	return MyPawn->State != EZombieState::EDying;
}

bool AZombieController::IsInAttackRange()
{
	if (MyTarget)
	{
		return MyPawn->ActorsInRange.Find(MyTarget) != INDEX_NONE;
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

	FVector TargetLocation = MyTarget->GetActorLocation();

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(ZombieLocation, TargetLocation);
	FRotator ZombieNewRotation = FMath::RInterpTo(ZombieRotation, LookAt, 0.1f, 2.0f);

	bool bFaceTarget = FMath::IsNearlyEqual(ZombieRotation.Yaw, ZombieNewRotation.Yaw, 0.001f);
	BlackboardComp->SetValueAsBool("bFaceTarget", bFaceTarget);

	ZombieRotation.Yaw = ZombieNewRotation.Yaw;
	MyPawn->SetActorRotation(ZombieRotation);
}

bool AZombieController::IsActorTargeted(AActor* Actor)
{
	return Actor == MyTarget;
}

bool AZombieController::CanZombieReachPlayer(AActor* Player)
{
	FVector PathStart = MyPawn->GetActorLocation();
	UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	UNavigationPath* NavPath = NavigationArea->FindPathToActorSynchronously(
		GetWorld(), 
		PathStart, 
		Player,
		NULL);

	if (!NavPath)
		return false;

	return !NavPath->IsPartial();
}

AActor* AZombieController::GetClosestBarricade()
{
	TArray<AActor*> FoundBarricades;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABarricade::StaticClass(), FoundBarricades);
	
	float ClosestDistance = std::numeric_limits<float>::max();
	AActor* ClosestBarricade = NULL;
	for (int i = 0; i < FoundBarricades.Num(); i++) {
		AActor* Barricade = FoundBarricades[i];
		float DistanceTo = MyPawn->GetDistanceTo(Barricade);
		if (DistanceTo < ClosestDistance)
		{
			ClosestDistance = DistanceTo;
			ClosestBarricade = Barricade;
		}
	}

	return ClosestBarricade;
}