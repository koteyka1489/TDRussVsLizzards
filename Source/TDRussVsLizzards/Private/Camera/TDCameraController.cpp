// TD Russ Vs Lizzards Game

#include "Camera/TDCameraController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraController, Display, Display);

ATDCameraController::ATDCameraController() {}

void ATDCameraController::BeginPlay()
{
    Super::BeginPlay();

    CameraPawn = Cast<ACameraPawn>(GetPawn());
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
        UE_LOG(LogCameraController, Error,
            TEXT(
                "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend "),
            *GetNameSafe(this));
    }
}

void ATDCameraController::MoveCameraUp()
{

    if (CameraPawn)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        CameraPawn->SetActorLocation(CameraPawn->GetActorLocation() + CameraPawn->GetActorForwardVector() * SpeedCamera * DeltaTime);
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" Camera pawn not initialize "));
        checkNoEntry();
    }
}

void ATDCameraController::MoveCameraDown()
{

    if (CameraPawn)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        CameraPawn->SetActorLocation(
            CameraPawn->GetActorLocation() + CameraPawn->GetActorForwardVector() * -1.0f * SpeedCamera * DeltaTime);
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" Camera pawn not initialize "));
        checkNoEntry();
    }
}

void ATDCameraController::MoveCameraRight()
{

    if (CameraPawn)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        CameraPawn->SetActorLocation(CameraPawn->GetActorLocation() + CameraPawn->GetActorRightVector() * SpeedCamera * DeltaTime);
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" Camera pawn not initialize "));
        checkNoEntry();
    }
}

void ATDCameraController::MoveCameraLeft()
{

    if (CameraPawn)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        CameraPawn->SetActorLocation(CameraPawn->GetActorLocation() + CameraPawn->GetActorRightVector() * -1.0f * SpeedCamera * DeltaTime);
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" Camera pawn not initialize "));
        checkNoEntry();
    }
}
