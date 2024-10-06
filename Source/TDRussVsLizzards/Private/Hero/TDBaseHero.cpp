// TD Russ Vs Lizzards Game

#include "Hero/TDBaseHero.h"
#include "Camera/TDCameraController.h"

// Sets default values
ATDBaseHero::ATDBaseHero()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATDBaseHero::BeginPlay()
{
    Super::BeginPlay();

    auto CameraController = Cast<ATDCameraController>(GetController());
    if (CameraController)
    {
        GEngine->AddOnScreenDebugMessage(3, 10, FColor::Red, TEXT("nSetHeroDestination BIND"));
        CameraController->OnSetHeroDestination.BindUObject(this, &ATDBaseHero::OnSetHeroDestination);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" CameraController Cast Fail "));
        checkNoEntry();
    }
}



// Called every frame
void ATDBaseHero::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATDBaseHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATDBaseHero::OnSetHeroDestination(bool OnHitSucefull, FVector HeroDestination) 
{
    auto MovementComponent = GetMovementComponent();
    FString Message        = FString::Printf(TEXT("OnHitSucefull - %s"), OnHitSucefull ? TEXT("TRUE") : TEXT("FALSE"));
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, Message);

    FString Message1 = FString::Printf(TEXT("VECTOR DESTINATION"), *HeroDestination.ToString());
    GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, Message1);

    if (OnHitSucefull)
    {

    }
    else
    {
        

    }
}