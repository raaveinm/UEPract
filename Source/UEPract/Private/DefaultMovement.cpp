//
// Created by raaveinm on 2/17/26.
//

#include "DefaultMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADefaultMovement::ADefaultMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// detaching camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// character setup
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("RTS_Camera"));
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying; 
	GetCharacterMovement()->MaxFlySpeed = 1200.f; 
	GetCharacterMovement()->BrakingDecelerationFlying = 2000.f;


    // camera setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = LENGTH;
	CameraBoom->SetRelativeRotation(FRotator(PITCH, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 3.f;
	
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;
	TopDownCamera->FieldOfView = FOV;

	// 3rd person character doesn't rotate with controller, but rotates with movement direction
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void ADefaultMovement::BeginPlay() {
	Super::BeginPlay();
	
	// Link Master - Slave
	controlledUnit = Cast<AUnitCharacter>(GetOwner());
	if (controlledUnit == nullptr) {
		AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(),AUnitCharacter::StaticClass());
		controlledUnit = Cast<AUnitCharacter>(findActor);
	}

	// Get the player controller and add input mapping context
	if (APlayerController* playerController = Cast<APlayerController>(Controller)) {
		playerController->bShowMouseCursor = true;
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;

		if (UEnhancedInputLocalPlayerSubsystem* subsys = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			if (DefaultMappingContext) {
				subsys->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

///////////////////////////////////////////////
// Movement handling
///////////////////////////////////////////////

void ADefaultMovement::Command(const FInputActionValue& val) {
	if (!controlledUnit) return;
	
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!playerController) return;
	
	FHitResult hit;
	if (playerController->GetHitResultUnderCursor(ECC_Visibility, false, hit)) {
		DrawDebugSphere(
			GetWorld(), hit.Location, 50.f, 12, FColor::Magenta, false, 1.f);
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controlledUnit->GetController(), hit.Location);
	}
}

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


void ADefaultMovement::Zoom(const FInputActionValue& val) {
	if (!CameraBoom) return;
	float zoom = val.Get<float>();
	float new_length = CameraBoom->TargetArmLength - (zoom * SPEED);
	CameraBoom->TargetArmLength = FMath::Clamp(new_length, MIN_ZOOM, MAX_ZOOM);
}

// Called every frame
void ADefaultMovement::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
	
	FHitResult hit;
	FVector start = GetActorLocation();
	FVector end = start - FVector(0,0,10000);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_GameTraceChannel1, params)) {
		float desiredHeight = 100.f;
		float currentHeight = hit.Distance;
		if (FMath::Abs(currentHeight - desiredHeight) > 1.f) {
			FVector newLoc = GetActorLocation();
			newLoc.Z = FMath::FInterpTo(newLoc.Z, hit.Location.Z + desiredHeight, DeltaTime, 10.f);
			
			SetActorLocation(newLoc);
		}
	}
}

// Called to bind functionality to input
void ADefaultMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInput) {
		if (MoveAction)
			enhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultMovement::Move);
		if (ZoomAction)
			enhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ADefaultMovement::Zoom);
		if (CommandAction)
			enhancedInput->BindAction(CommandAction, ETriggerEvent::Started, this, &ADefaultMovement::Command);
	}
}