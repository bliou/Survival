// Fill out your copyright notice in the Description page of Project Settings.

#include "Barricade.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABarricade::OnStartColliding);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABarricade::OnEndColliding);
	
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
	bIsPlaced = false;

	EquippedWidget = CreateWidget<UUserWidget>(GetWorld(), wEquippedWidget);

	// Get the dynamic material
	DynamicMaterial = UMaterialInstanceDynamic::Create(BarricadeMesh->GetMaterial(0), this);
}

// Called every frame
void ABarricade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingLeft)
		RotateBarricadeLeft();
	if (bIsRotatingRight)
		RotateBarricadeRight();

	if (!bIsPlaced)
	{
		if (NumActorsCollided != 0)
			DynamicMaterial->SetVectorParameterValue(FName("MaterialColor"), FLinearColor::Red);
		else
			DynamicMaterial->SetVectorParameterValue(FName("MaterialColor"), FLinearColor::Green);

		BarricadeMesh->SetMaterial(0, DynamicMaterial);
	}
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
	BarricadeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EquippedWidget->AddToViewport();

	DynamicMaterial->SetVectorParameterValue(FName("MaterialColor"), FLinearColor::Green);
	BarricadeMesh->SetMaterial(0, DynamicMaterial);
}

bool ABarricade::Place()
{
	if (NumActorsCollided != 0)
		return false;
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	BarricadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	EquippedWidget->RemoveFromParent();
	bIsPlaced = true;
	DynamicMaterial->SetVectorParameterValue(FName("MaterialColor"), FLinearColor::Black);
	BarricadeMesh->SetMaterial(0, DynamicMaterial);

	return true;
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

void ABarricade::OnStartColliding(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
	if (Player)
		return;

	NumActorsCollided++;
}

void ABarricade::OnEndColliding(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
	if (Player)
		return;

	NumActorsCollided--;
}