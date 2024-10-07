// TD Russ Vs Lizzards Game

#include "Hero/TDBaseHero.h"
#include "Camera/TDCameraController.h"
#include "Kismet\GameplayStatics.h"


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



// Called every frame
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

    FString Message1 = FString::Printf(TEXT("VECTOR DESTINATION - %s"), *HeroDestination.ToString());
    GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, Message1);

}