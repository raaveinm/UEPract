// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultMovement.generated.h"

UCLASS()
class UEPRACT_API ADefaultMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	///////////////////////////////////////////////
	// WSAD movement
	///////////////////////////////////////////////

	// mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	// movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;

	// handling movement
	void Move(const struct FInputActionValue& val);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
