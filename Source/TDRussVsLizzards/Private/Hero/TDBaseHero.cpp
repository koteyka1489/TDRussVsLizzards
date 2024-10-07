// TD Russ Vs Lizzards Game

#include "Hero/TDBaseHero.h"
#include "Camera/TDCameraController.h"
#include "Kismet\GameplayStatics.h"
#include "AI/TDAIController.h"


ATDBaseHero::ATDBaseHero()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATDBaseHero::BeginPlay()
{
    Super::BeginPlay();

    auto CameraController = Cast<ATDCameraController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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

void ATDBaseHero::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATDBaseHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATDBaseHero::OnSetHeroDestination( FVector HeroDestination) 
{
    auto ControllerAI = Cast<ATDAIController>(GetController());
    if (ControllerAI)
    {
        ControllerAI->MoveToLocation(HeroDestination);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" Cast to Ai controller Fail "));
        checkNoEntry();
    }

}