// TD Russ Vs Lizzards Game

#include "Camera/TDCameraController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraController, Display, Display);

ATDCameraController::ATDCameraController() {}

void ATDCameraController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor       = true;
    bEnableClickEvents     = true;
    bEnableMouseOverEvents = true;

    
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
        EnhancedInputComponent->BindAction(MoveCameraUpAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUpDown);
        EnhancedInputComponent->BindAction(MoveCameraDownAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUpDown);
        EnhancedInputComponent->BindAction(
            MoveCameraRightAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
        EnhancedInputComponent->BindAction(MoveCameraLeftAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
        EnhancedInputComponent->BindAction(ZoomUpCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
        EnhancedInputComponent->BindAction(ZoomDownCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
        EnhancedInputComponent->BindAction(RotateRightCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::RotateCamera);
        EnhancedInputComponent->BindAction(RotateLeftCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::RotateCamera);

        EnhancedInputComponent->BindAction(
            SetHeroDestinationAction, ETriggerEvent::Triggered, this, &ATDCameraController::SetHeroDestinationTriggered);
    }
    else
    {
        UE_LOG(LogCameraController, Error,
            TEXT(
                "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend "),
            *GetNameSafe(this));
    }
}

void ATDCameraController::MoveCameraUpDown(const FInputActionValue& Value)
{
    if (OnMoveCameraUpDown.ExecuteIfBound(Value.Get<float>()))
    {
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" OnMoveCameraUpDown Delegate is not bound "));
        checkNoEntry();
    }
}

void ATDCameraController::MoveCameraRightLeft(const FInputActionValue& Value)
{
    if (OnMoveCameraRightLeft.ExecuteIfBound(Value.Get<float>()))
    {
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" OnMoveCameraRightLeft Delegate is not bound "));
        checkNoEntry();
    }
}

void ATDCameraController::ZoomUpAction(const FInputActionValue& Value)
{
    if (OnZoomChanged.ExecuteIfBound(Value.Get<float>()))
    {
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" OnZoomChanged Delegate is not bound "));
        checkNoEntry();
    }
}

void ATDCameraController::RotateCamera(const FInputActionValue& Value)
{
    if (OnRotateCamera.ExecuteIfBound(Value.Get<float>()))
    {
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" OnRotateCamera Delegate is not bound "));
        checkNoEntry();
    }
}

void ATDCameraController::SetHeroDestinationTriggered()
{
    std::pair<bool, FVector> HitResultAction = GetHeroDestination();
    bool bHitSuccessfull                      = HitResultAction.first;
    FVector HeroDestination                  = HitResultAction.second;
    if (OnSetHeroDestination.ExecuteIfBound(bHitSuccessfull, HeroDestination))
    {
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" OnSetHeroDestination Delegate is not bound "));
        checkNoEntry();
    }
}

std::pair<bool, FVector> ATDCameraController::GetHeroDestination()
{
    FHitResult Hit;
    bool bHitSuccessful = false;
    bHitSuccessful      = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
    if (bHitSuccessful)
    {
        return {bHitSuccessful, Hit.Location};
    }
    return {bHitSuccessful, FVector{}};
}
