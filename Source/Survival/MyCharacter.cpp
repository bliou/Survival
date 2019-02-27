// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Engine.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SurvivalGameStateBase.h"
#include "SurvivalGameModeBase.h"

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

	Inventory = NewObject<UInventory>();
	Inventory->Initialize(GetWorld());

	//StartWave();
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

	InteractWithBarricade();
}

void AMyCharacter::StartWave()
{
	ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
	if (GameState->CurrentState == EGameState::EInBetweenWaves)
	{
		if (!CurrentWeapon)
			Inventory->EquipPreviousWeapon();

		GameState->CurrentState = EGameState::EStartWave;
	}
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
	PlayerInputComponent->BindAction("EquipFirstItem", IE_Pressed, this, &AMyCharacter::EquipFirstItem);
	PlayerInputComponent->BindAction("EquipSecondItem", IE_Pressed, this, &AMyCharacter::EquipSecondItem);
	PlayerInputComponent->BindAction("EquipThirdItem", IE_Pressed, this, &AMyCharacter::EquipThirdItem);
	PlayerInputComponent->BindAction("UnEquip", IE_Pressed, this, &AMyCharacter::UnEquip);
	PlayerInputComponent->BindAction("EquipPreviousWeapon", IE_Pressed, this, &AMyCharacter::EquipPreviousWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::StartReloading);
	PlayerInputComponent->BindAction("StartWave", IE_Pressed, this, &AMyCharacter::StartWave);
	PlayerInputComponent->BindAction("Buy", IE_Pressed, this, &AMyCharacter::Buy);

	PlayerInputComponent->BindAction("BarricadeInteraction", IE_Pressed, this, &AMyCharacter::BarricadeInteraction);
	PlayerInputComponent->BindAction("RotateBarricadeLeft", IE_Pressed, this, &AMyCharacter::RotateBarricadeLeft);
	PlayerInputComponent->BindAction("RotateBarricadeLeft", IE_Released, this, &AMyCharacter::StopRotateBarricadeLeft);
	PlayerInputComponent->BindAction("RotateBarricadeRight", IE_Pressed, this, &AMyCharacter::RotateBarricadeRight);
	PlayerInputComponent->BindAction("RotateBarricadeRight", IE_Released, this, &AMyCharacter::StopRotateBarricadeRight);
}

void AMyCharacter::AddControllerYawInput(float Val)
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (State == ECharacterState::EDead
		|| GameMode->CurrentWidgetType != EWidgetType::EInGame)
		return;

	Super::AddControllerYawInput(Val);
}

void AMyCharacter::AddControllerPitchInput(float Val)
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (State == ECharacterState::EDead
		|| GameMode->CurrentWidgetType != EWidgetType::EInGame)
		return;

	Super::AddControllerPitchInput(Val);
}

void AMyCharacter::MoveForward(float Value)
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (State == ECharacterState::EDead
		|| GameMode->CurrentWidgetType != EWidgetType::EInGame)
		return;

	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (State == ECharacterState::EDead
		|| GameMode->CurrentWidgetType != EWidgetType::EInGame)
		return;

	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::Fire()
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (CurrentWeapon
		&& GameMode->CurrentWidgetType == EWidgetType::EInGame
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
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (State == ECharacterState::EIdle
		&& CurrentWeapon
		&& GameMode->CurrentWidgetType == EWidgetType::EInGame)
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

void AMyCharacter::EquipFirstItem()
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode->CurrentWidgetType == EWidgetType::EInGame)
	{
		ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
		if (GameState->CurrentState == EGameState::EInBetweenWaves)
			Inventory->EquipBarricade(0);
		else
			Inventory->EquipWeapon(0);
	}
}

void AMyCharacter::EquipSecondItem()
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode->CurrentWidgetType == EWidgetType::EInGame)
	{
		ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
		if (GameState->CurrentState == EGameState::EInBetweenWaves)
			Inventory->EquipBarricade(1);
		else
			Inventory->EquipWeapon(1);
	}
}

void AMyCharacter::EquipThirdItem()
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode->CurrentWidgetType == EWidgetType::EInGame)
	{
		ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
		if (GameState->CurrentState == EGameState::EInBetweenWaves)
			Inventory->EquipBarricade(2);
		else
			Inventory->EquipWeapon(2);
	}
}

void AMyCharacter::UnEquip()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->UnEquip();
		CurrentWeapon = NULL;
	}
	if (CurrentBarricade)
	{
		Inventory->UnEquipBarricade();
		CurrentBarricade = NULL;
	}
}


void AMyCharacter::EquipPreviousWeapon()
{
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode->CurrentWidgetType == EWidgetType::EInGame)
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

void AMyCharacter::Buy()
{
	ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
	if (GameState->CurrentState == EGameState::EInBetweenWaves)
	{
		ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->SwitchWidget(EWidgetType::EShop);
	}
}

void AMyCharacter::BarricadeInteraction()
{
	if (CurrentBarricade)
	{
		if (CurrentBarricade->Place())
			CurrentBarricade = NULL;
	}
	else
	{
		if (InterractableBarricade)
		{
			InterractableBarricade->Equip(this);
			InterractableBarricade->InterractWidgetComponent->SetVisibility(false);
			CurrentBarricade = InterractableBarricade;
			InterractableBarricade = NULL;
		}
	}
}

void AMyCharacter::RotateBarricadeLeft()
{
	if (!CurrentBarricade)
		return;
	CurrentBarricade->RotateBarricadeLeft();
	CurrentBarricade->bIsRotatingLeft = true;
}

void AMyCharacter::StopRotateBarricadeLeft()
{
	if (CurrentBarricade)
		CurrentBarricade->bIsRotatingLeft = false;
}

void AMyCharacter::RotateBarricadeRight()
{
	if (!CurrentBarricade)
		return;
	CurrentBarricade->RotateBarricadeRight();
	CurrentBarricade->bIsRotatingRight = true;
}

void AMyCharacter::StopRotateBarricadeRight()
{
	if (CurrentBarricade)
		CurrentBarricade->bIsRotatingRight = false;
}

void AMyCharacter::InteractWithBarricade()
{
	if (InterractableBarricade)
	{
		InterractableBarricade->BarricadeMesh->SetRenderCustomDepth(false);
		InterractableBarricade->InterractWidgetComponent->SetVisibility(false);
		InterractableBarricade = NULL;
	}

	ASurvivalGameStateBase* GameState = GetWorld()->GetGameState<ASurvivalGameStateBase>();
	ASurvivalGameModeBase* GameMode = Cast<ASurvivalGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameState->CurrentState != EGameState::EInBetweenWaves
		|| GameMode->CurrentWidgetType != EWidgetType::EInGame
		|| CurrentBarricade)
	{
		return;
	}


	FVector StartTrace = FPSCameraComponent->GetComponentLocation();
	FVector EndTrace = FPSCameraComponent->GetForwardVector() * 300.f + StartTrace;

	static FName PlayerBarricadeTag = FName(TEXT("PlayerInteractBarricade"));

	FCollisionQueryParams TraceParams(PlayerBarricadeTag, true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartTrace,
		EndTrace,
		ECollisionChannel::ECC_PhysicsBody,
		TraceParams);

	if (Hit.GetActor())
	{
		ABarricade* barricade = Cast<ABarricade>(Hit.GetActor());
		if (barricade)
		{
			InterractableBarricade = barricade;
			InterractableBarricade->BarricadeMesh->SetRenderCustomDepth(true);
			InterractableBarricade->InterractWidgetComponent->SetVisibility(true);
		}
	}
}