// Fill out your copyright notice in the Description page of Project Settings.

#include "DroppedItem.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "MyCharacter.h"

// Sets default values
ADroppedItem::ADroppedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADroppedItem::OnCollide);
	RootComponent = Cast<USceneComponent>(CollisionComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroppedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADroppedItem::OnCollide(
	class UPrimitiveComponent* HitComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		switch (DroppedItemType)
		{
		case EDroppedItemType::ELife:
			MyCharacter->CharacterConfig.CurrentHealth = MyCharacter->CharacterConfig.MaxHealth;
			break;
		case EDroppedItemType::EMoney:
		{
			// TODO : Use a DataTable to select the money to add
			MyCharacter->CharacterConfig.CurrentMoney += 300;
			break;
		}
		}

		Destroy();
	}
}