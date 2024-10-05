// TD Russ Vs Lizzards Game


#include "Camera/TDCameraController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"

ATDCameraController::ATDCameraController() {}

void ATDCameraController::BeginPlay() 
{
    Super::BeginPlay();
}

void ATDCameraController::SetupInputComponent() 
{
    Super::SetupInputComponent();

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(MoveCameraUpAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUp);
        EnhancedInputComponent->BindAction(MoveCameraDownAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraDown);
        EnhancedInputComponent->BindAction(MoveCameraRightAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRight);
        EnhancedInputComponent->BindAction(MoveCameraLeftAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraLeft);
    }
    else
    {
        UE_LOG(LogTemp, Error,
            TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend "
                 "to use the legacy system, then you will need to update this C++ file."),
            *GetNameSafe(this));
    }
}

void ATDCameraController::MoveCameraUp() 
{
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("UP"));
}

void ATDCameraController::MoveCameraDown() 
{
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("DOWN"));
}

void ATDCameraController::MoveCameraRight() 
{
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("RIGHT"));
}

void ATDCameraController::MoveCameraLeft()
{
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("LEFT"));
}


