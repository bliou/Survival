// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Engine.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComponent->SetupAttachment(GetCapsuleComponent());
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnBeginOverlap);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(
		GetMesh(),
		TEXT("Head")
	);
	FPSCameraComponent->bUsePawnControlRotation = true;

	// By default, the player has a gun
	Weapons.SetNum(2, false);
	PreviousWeaponSlot = -1;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipDefaultWeapon();

	CharacterConfig.CurrentHealth = CharacterConfig.MaxHealth;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReloading)
	{
		ReloadTimer -= DeltaTime;
		if (ReloadTimer <= 0.f)
			bIsReloading = false;
	}
}

void AMyCharacter::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	//AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	//if (Weapon)
	//{
	//	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
	//	if (Weapon->WeaponType == EWeaponType::EHeavyWeapon)
	//	{
	//		Weapons[1] = Spawner;
	//	}

	//	Spawner->SetOwningPawn(this);
	//	Spawner->UnEquip();
	//	Weapon->Destroy();
	//}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
	PlayerInputComponent->BindAction("EquipGun", IE_Pressed, this, &AMyCharacter::EquipGun);
	PlayerInputComponent->BindAction("EquipHeavyWeapon", IE_Pressed, this, &AMyCharacter::EquipHeavyWeapon);
	PlayerInputComponent->BindAction("EquipPreviousWeapon", IE_Pressed, this, &AMyCharacter::EquipPreviousWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::Reload);
}

void AMyCharacter::MoveForward(float Value)
{
	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::Fire()
{
	//// try and play a firing animation if specified
	//if (FireAnimation != NULL)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != NULL)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}

	CurrentWeapon->Fire();
}

void AMyCharacter::Reload()
{
	CurrentWeapon->Reload();
}

void AMyCharacter::EquipDefaultWeapon()
{
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	if (Spawner)
	{
		Weapons[0] = Spawner;
		CurrentWeapon = Spawner;
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->Equip();
	}
}

void AMyCharacter::EquipGun()
{
	if (!Weapons[0])
		return;
	if (CurrentWeapon)
	{
		if (CurrentWeapon->WeaponType == EWeaponType::EGun)
			return;

		int Slot = CurrentWeaponSlot();
		PreviousWeaponSlot = Slot;
		CurrentWeapon->UnEquip();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString(TEXT("Put away " + CurrentWeapon->WeaponConfig.Name + " in slot " + FString::FromInt(Slot))));
	}

	CurrentWeapon = Weapons[0];
	CurrentWeapon->SetOwningPawn(this);
	CurrentWeapon->Equip();
}

void AMyCharacter::EquipHeavyWeapon()
{
	if (!Weapons[1])
		return;

	if (CurrentWeapon)
	{
		if (CurrentWeapon->WeaponType == EWeaponType::EHeavyWeapon)
			return;

		int Slot = CurrentWeaponSlot();
		PreviousWeaponSlot = Slot;
		CurrentWeapon->UnEquip();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString(TEXT("Put away " + CurrentWeapon->WeaponConfig.Name + " in slot " + FString::FromInt(Slot))));
	}

	CurrentWeapon = Weapons[1];
	CurrentWeapon->SetOwningPawn(this);
	CurrentWeapon->Equip();
}

void AMyCharacter::EquipPreviousWeapon()
{
	switch (PreviousWeaponSlot)
	{
	case 0:
		EquipGun();
		break;
	case 1:
		EquipHeavyWeapon();
		break;
	}
}

int AMyCharacter::CurrentWeaponSlot()
{
	if (CurrentWeapon)
	{
		switch (CurrentWeapon->WeaponType)
		{
		case EWeaponType::EGun:
			return 0;
		case EWeaponType::EHeavyWeapon:
			return 1;
		}
	}
	return -1;
}