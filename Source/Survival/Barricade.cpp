// Fill out your copyright notice in the Description page of Project Settings.

#include "Barricade.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = Cast<USceneComponent>(CollisionComponent);

	BarricadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarricadeMesh"));
	BarricadeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
	BarricadeConfig.CurrentHealth = BarricadeConfig.MaxHealth;
	bIsRotatingLeft = false;
	bIsRotatingRight = false;
}

// Called every frame
void ABarricade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingLeft)
		RotateBarricadeLeft();
	if (bIsRotatingRight)
		RotateBarricadeRight();
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

void ABarricade::Equip(AMyCharacter* MyCharacter)
{
	AttachToActor(
		MyCharacter,
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
	);
	SetActorRelativeScale3D(FVector(1.1f, 2.0f, 2.0f));
	SetActorRelativeLocation(FVector(100.0f, 0.f, 0.f));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BarricadeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABarricade::RotateBarricadeLeft()
{
	FRotator RelativeRotation = GetActorRotation();
	RelativeRotation.Yaw += 2.f;
	SetActorRotation(RelativeRotation);
}

void ABarricade::RotateBarricadeRight()
{
	FRotator RelativeRotation = GetActorRotation();
	RelativeRotation.Yaw -= 2.f;
	SetActorRotation(RelativeRotation);
}