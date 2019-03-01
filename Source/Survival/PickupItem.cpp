// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "MyCharacter.h"

// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnCollide);
	RootComponent = Cast<USceneComponent>(CollisionComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	LifeSpan = 20.f;
	SetLifeSpan(LifeSpan);
	ItemMesh->SetRenderCustomDepth(true);

	ToggleVisibilityTimer = 1.f;
	ToggleVisibilityCurrentTimer = ToggleVisibilityTimer;
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Yaw += 50.f * DeltaTime;

	SetActorRotation(ActorRotation);

	LifeSpan -= DeltaTime;
	if (LifeSpan <= 10.f)
	{
		if (ToggleVisibilityCurrentTimer <= 0.f)
		{
			ToggleVisibilityTimer -= 0.05f;
			ToggleVisibilityCurrentTimer = ToggleVisibilityTimer;
			ItemMesh->ToggleVisibility();
		}
		ToggleVisibilityCurrentTimer -= DeltaTime;
	}
}

void APickupItem::OnCollide(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		switch (PickupItemConfig.PickupItemType)
		{
		case EPickupItemType::ELife:
			MyCharacter->CharacterConfig.CurrentHealth = MyCharacter->CharacterConfig.MaxHealth;
			break;
		case EPickupItemType::EMoney:
			MyCharacter->CharacterConfig.CurrentMoney += PickupItemConfig.Value;
			break;
		case EPickupItemType::EAmmo:
			MyCharacter->Inventory->AddAmmoToRandomWeapon(PickupItemConfig.Value);
			break;
		}

		Destroy();
	}
}