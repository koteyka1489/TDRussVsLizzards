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
}

void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();

    FVector StartingCameraLocation(0.0, 0.0, 1200.0);
    FRotator StartingCameraRotation(0.0, 0.0, 0.0);
    SetActorLocationAndRotation(StartingCameraLocation, StartingCameraRotation.Quaternion());
}

void ACameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveUp", this, &ACameraPawn::MoveCameraUp);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveCameraRight);
}

void ACameraPawn::MoveCameraUp(float Amount)
{
    GEngine->AddOnScreenDebugMessage(1, 0.5, FColor::Red, TEXT("UP"));

    FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Amount * SpeedCamera;
    SetActorLocation(NewLocation);
}

void ACameraPawn::MoveCameraRight(float Amount)
{
    GEngine->AddOnScreenDebugMessage(2, 0.5, FColor::Red, TEXT("Right"));

    FVector NewLocation = GetActorLocation() + GetActorRightVector() * Amount * SpeedCamera;
    SetActorLocation(NewLocation);
}
