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

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	// Allow the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);
	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// Disable some environmental shadowing to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// By default, the player has a gun
	Weapons.SetNum(2, false);
	PreviousWeaponSlot = -1;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipDefaultWeapon();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if (CurrentWeapon)
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