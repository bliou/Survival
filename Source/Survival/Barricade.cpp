// Fill out your copyright notice in the Description page of Project Settings.

#include "Barricade.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = dynamic_cast<USceneComponent*>(CollisionComponent);

	BarricadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarricadeMesh"));
	BarricadeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
	BarricadeConfig.CurrentHealth = BarricadeConfig.MaxHealth;
}

// Called every frame
void ABarricade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABarricade::TakeDamages(float Damages)
{
	BarricadeConfig.CurrentHealth -= Damages;
	if (BarricadeConfig.CurrentHealth <= 0.f)
		Destroy();
}

void ABarricade::HittenByBullet(
	const FHitResult& Impact,
	const FVector& ShootDir)
{
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HittenParticles,
		Impact.Location,
		ShootDir.Rotation().GetInverse(),
		true
	);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		BulletHitSound,
		Impact.Location
	);
}