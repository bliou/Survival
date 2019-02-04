// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Engine.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsDying = false;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = Cast<USceneComponent>(MeshComp);
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
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
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString(TEXT("Zombie loses " + FString::FromInt(GunDamage) + " Health remains : " + FString::FromInt(ZombieConfig.Health))));
	if (ZombieConfig.Health <= 0)
		bIsDying = true;
}