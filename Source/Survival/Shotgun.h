// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API AShotgun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AShotgun();

	virtual void Reload() override;
};
