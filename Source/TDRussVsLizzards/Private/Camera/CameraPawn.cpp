// TD Russ Vs Lizzards Game

#include "Camera/CameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACameraPawn::ACameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    FVector StartingCameraLocation(0.0, 0.0, 0.0);
    FRotator StartingCameraRotation(0.0, 0.0, 0.0);
    SetActorLocationAndRotation(StartingCameraLocation, StartingCameraRotation.Quaternion());
}

void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ACameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("Up", EInputEvent::IE_Pressed, this, &ACameraPawn::MoveCameraUp);
    PlayerInputComponent->BindAction("Down", EInputEvent::IE_Pressed, this, &ACameraPawn::MoveCameraDown);
    PlayerInputComponent->BindAction("Right", EInputEvent::IE_Pressed, this, &ACameraPawn::MoveCameraRight);
    PlayerInputComponent->BindAction("Left", EInputEvent::IE_Pressed, this, &ACameraPawn::MoveCameraLeft);
}

void ACameraPawn::MoveCameraUp() 
{
    GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, TEXT("UP"));
}

void ACameraPawn::MoveCameraDown()
{
    GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, TEXT("Down"));
}

void ACameraPawn::MoveCameraRight()
{
    GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, TEXT("Right"));
}

void ACameraPawn::MoveCameraLeft()
{
    GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, TEXT("Left"));
}