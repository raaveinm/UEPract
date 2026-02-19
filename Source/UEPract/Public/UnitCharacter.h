// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitCharacter.generated.h"

///////////////////////////////////////////////
// Slave character
///////////////////////////////////////////////

UCLASS()
class UEPRACT_API AUnitCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AUnitCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
