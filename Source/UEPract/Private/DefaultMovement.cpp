//
// Created by raaveinm on 2/17/26.
//

#include "DefaultMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADefaultMovement::ADefaultMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 3rd person character doesn't rotate with controller, but rotates with movement direction
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void ADefaultMovement::BeginPlay() {
	Super::BeginPlay();

	// Get the player controller and add input mapping context
	if (APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			if (DefaultMappingContext) {
				subsys->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

///////////////////////////////////////////////
// Movement handling
///////////////////////////////////////////////

void ADefaultMovement::Move(const FInputActionValue& val) {
	if (Controller == nullptr) return;

	FVector2D movementVector = val.Get<FVector2D>();

	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(forwardDirection, movementVector.Y);
	AddMovementInput(rightDirection, movementVector.X);
}

// Called every frame
void ADefaultMovement::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADefaultMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInput && MoveAction)
		enhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultMovement::Move);
}

