// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "UnitCharacter.h"
#include "LevelControl.h"
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
	// Camera Control
	///////////////////////////////////////////////
	
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	class UCameraComponent* TopDownCamera;
	
	// const
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	float MIN_ZOOM = 300.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	float MAX_ZOOM = 1500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	float SPEED = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	float LENGTH = 700.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	float PITCH = -60.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera")
	float FOV = 90.f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Control")
	AUnitCharacter* controlledUnit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Control")
	class UInputAction* CommandAction;
	
	void Command(const FInputActionValue& val);
	
	///////////////////////////////////////////////
	// WSAD movement
	///////////////////////////////////////////////
	
	// mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	// movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;
	
	// zoom
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ZoomAction;

	// handling movement & zoom
	void Move(const struct FInputActionValue& val);
	void Zoom(const struct FInputActionValue& val);
	
	///////////////////////////////////////////////
	// Level Manager
	///////////////////////////////////////////////
	
	UPROPERTY()
	ALevelControl* LevelManager;
	void ChangeFloorUp(const FInputActionValue& val);
	void ChangeFloorDown(const FInputActionValue& val);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
