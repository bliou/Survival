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
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	State = ECharacterState::EIdle;
	CharacterConfig.CurrentHealth = CharacterConfig.MaxHealth;

	Inventory = GetWorld()->SpawnActor<AInventory>(Inventory_BP);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == ECharacterState::EFire)
	{
		if (CurrentWeapon->WeaponConfig.bAutoFire
			&& !CurrentWeapon->bIsRecoiling)
			CurrentWeapon->Fire();
	}

	// If the player is moving, spread the weapon to its max value
	if ((GetVelocity().X != 0.f
		|| GetVelocity().Y != 0.f)
		&& CurrentWeapon)
	{
		CurrentWeapon->WeaponConfig.CurrentWeaponSpread = 
			FMath::Max(CurrentWeapon->WeaponConfig.CurrentWeaponSpread, CurrentWeapon->WeaponConfig.WeaponMaxSpread / 2.f);
	}
}

void AMyCharacter::StartWave()
{
	bCanEquipWeapon = true;
	Inventory->EquipPreviousWeapon();
}

void AMyCharacter::EndWave()
{
	bCanEquipWeapon = false;
	Inventory->SetPreviousWeaponSlot(CurrentWeapon->WeaponType);
	CurrentWeapon->UnEquip();
	CurrentWeapon = NULL;
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
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::StopAutoFiring);
	PlayerInputComponent->BindAction("EquipGun", IE_Pressed, this, &AMyCharacter::EquipGun);
	PlayerInputComponent->BindAction("EquipShotgun", IE_Pressed, this, &AMyCharacter::EquipShotgun);
	PlayerInputComponent->BindAction("EquipPreviousWeapon", IE_Pressed, this, &AMyCharacter::EquipPreviousWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::StartReloading);
}

void AMyCharacter::AddControllerYawInput(float Val)
{
	if (State == ECharacterState::EDead)
		return;

	Super::AddControllerYawInput(Val);
}

void AMyCharacter::AddControllerPitchInput(float Val)
{
	if (State == ECharacterState::EDead)
		return;

	Super::AddControllerPitchInput(Val);
}

void AMyCharacter::MoveForward(float Value)
{
	if (State == ECharacterState::EDead)
		return;

	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	if (State == ECharacterState::EDead)
		return;

	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::Fire()
{
	if (CurrentWeapon
		&& (State == ECharacterState::EIdle
			|| (State == ECharacterState::EReload
				&& CurrentWeapon->WeaponType == EWeaponType::EShotgun)))
	{
		CurrentWeapon->Fire();
		State = ECharacterState::EFire;
	}
}

void AMyCharacter::StopAutoFiring()
{
	if (State == ECharacterState::EFire)
		State = ECharacterState::EIdle;
}

void AMyCharacter::StartReloading()
{
	if (State == ECharacterState::EIdle
		&& CurrentWeapon)
		CurrentWeapon->StartReloading();
}

void AMyCharacter::Reload()
{
	if (CurrentWeapon)
		CurrentWeapon->Reload();
}

void AMyCharacter::EndReloading()
{
	if (State != ECharacterState::EDead)
		State = ECharacterState::EIdle;
}

void AMyCharacter::TakeDamages(float Damages)
{
	if (State == ECharacterState::EDead)
		return;

	CharacterConfig.CurrentHealth -= Damages;

	UGameplayStatics::PlaySoundAtLocation(
		this,
		PlayerDamagedSound,
		GetActorLocation()
	);

	FTimerHandle UnusedHandle;
	if (CharacterConfig.CurrentHealth > 0.f)
	{
		if (!DamagedWidget)
			DamagedWidget = CreateWidget<UUserWidget>(GetWorld(), wDamagedWidget);
		DamagedWidget->AddToViewport();
		GetWorldTimerManager().SetTimer(
			UnusedHandle,
			this,
			&AMyCharacter::EndTakeDamages,
			1.f,
			false);
	}
	else
	{
		if (!DeadWidget)
			DeadWidget = CreateWidget<UUserWidget>(GetWorld(), wDeadWidget);
		DeadWidget->AddToViewport();
		GetWorldTimerManager().SetTimer(
			UnusedHandle,
			this,
			&AMyCharacter::KillPlayer,
			CharacterConfig.DyingAnimationLength,
			false);
		State = ECharacterState::EDead;
	}
}

void AMyCharacter::EquipGun()
{
	if (bCanEquipWeapon)
		Inventory->EquipWeapon(0);
}

void AMyCharacter::EquipShotgun()
{
	if (bCanEquipWeapon)
		Inventory->EquipWeapon(1);
}


void AMyCharacter::EquipPreviousWeapon()
{
	if (bCanEquipWeapon)
		Inventory->EquipPreviousWeapon();
}

void AMyCharacter::EndEquipping()
{
	if (State != ECharacterState::EDead)
		State = ECharacterState::EIdle;

	if (!CurrentWeapon)
		return;
	if (!CurrentWeapon->ReticleWidget)
		CurrentWeapon->ReticleWidget = CreateWidget<UUserWidget>(GetWorld(), CurrentWeapon->wReticleWidget);
	CurrentWeapon->ReticleWidget->AddToViewport();
}

void AMyCharacter::EndTakeDamages()
{
	DamagedWidget->RemoveFromParent();
}

void AMyCharacter::KillPlayer()
{
}