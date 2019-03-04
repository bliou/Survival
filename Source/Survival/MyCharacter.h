// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Inventory.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MyCharacter.generated.h"


UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	EIdle			UMETA(DisplayName = "Idle"),
	EReload			UMETA(DisplayName = "Reload"),
	EEquip			UMETA(DisplayName = "Equip"),
	EFire			UMETA(DisplayName = "Fire"),
	EDead			UMETA(DisplayName = "Dead")
};


USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	int32 CurrentMoney;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Animation)
	float DyingAnimationLength;
};

UCLASS()
class SURVIVAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Reload();

	// FPS camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	FCharacterData CharacterConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = State)
	ECharacterState State;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Sounds)
	class USoundBase* PlayerDamagedSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	AWeapon *CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Barricade)
	ABarricade * CurrentBarricade;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Inventory)
	UInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> wDamagedWidget;
	class UUserWidget* DamagedWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> wDeadWidget;
	class UUserWidget* DeadWidget;

	// Overridde theses methods to prevent the user from turning
	// the head of the player when he is dead
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;

	void TakeDamages(float Damages);

	void EndEquipping();
	void EndReloading();

	void StartWave();
	void EndWave();

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Fire();
	void StopAutoFiring();
	void StartReloading();

	void EquipFirstItem();
	void EquipSecondItem();
	void EquipThirdItem();

	void UnEquip();

	void EquipPreviousWeapon();

	void BarricadeInteraction();
	void RotateBarricadeLeft();
	void StopRotateBarricadeLeft();
	void RotateBarricadeRight();
	void StopRotateBarricadeRight();

	void InteractWithBarricade();
	ABarricade* InterractableBarricade;

	void OpenShop();
	void ExitShop();

	void EndTakeDamages();
	void KillPlayer();
};
